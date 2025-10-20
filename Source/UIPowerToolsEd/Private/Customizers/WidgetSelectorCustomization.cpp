// Copyright (c) Joel Gonzales


#include "Customizers/WidgetSelectorCustomization.h"
#include "UI/Screens/Tools/WidgetSelector.h"
#include "Components/Widget.h"
#include "BLueprint/WidgetTree.h"
#include "UI/Screens/Screen.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

static const FName EmptyEntry = FName(TEXT("<None>"));
static const FString Delimit = TEXT("/");

using namespace Customization;

FWidgetNode::FWidgetNode() 
	:WidgetName(EmptyEntry)
	,Path()
{}

FWidgetNode::FWidgetNode(FName InName) 
	:WidgetName(InName)
	,Path()
{}

FWidgetNode::FWidgetNode(FName InName, const TArray<FName>& InPath) 
	:WidgetName(InName)
	,Path(InPath) 
{}

bool FWidgetNode::operator==(const FWidgetNode& rhs) const
{
	return (this->WidgetName == rhs.WidgetName) && (this->Path == rhs.Path);
}

FText FWidgetNode::DisplayName() const 
{ 
	FString RetVal;
	for (const FName& PathName : Path)
	{
		RetVal.Append(PathName.ToString());
		RetVal.Append(Delimit);
	}

	RetVal.Append(WidgetName.ToString());
	return FText::FromString(RetVal);
}

bool FWidgetSelectorCustomization::FilterEntry(const UObject* Entry) const
{
	bool bRetVal = FBaseSelectorCustomization::FilterEntry(Entry);
	const TSubclassOf<UObject>& Filter = GetClassFilter();
	
	// if we're filtering panelwidget or derived, then check CanAddMoreChildren
	if (bRetVal && Filter && Filter->IsChildOf<UPanelWidget>())
	{
		if (const UPanelWidget* Panel = Cast<UPanelWidget>(Entry))
		{
			bRetVal = Panel->CanAddMoreChildren();
		}
	}

	return bRetVal;
}

TSharedRef<IPropertyTypeCustomization> FWidgetSelectorCustomization::MakeInstance()
{
	return MakeShared<FWidgetSelectorCustomization>();
}

void FWidgetSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	FBaseSelectorCustomization::CustomizeHeader(PropertyHandle, HeaderRow, CustomizationUtils);

	TSharedPtr<IPropertyHandle> ClassFilterProperty = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWidgetSelector, ClassFilter));
	if (ClassFilterProperty && ClassFilterProperty->IsValidHandle())
	{
		UObject* Object;
		ClassFilterProperty->GetValue(Object);
		if (Object)
		{
			ClassFilter = Cast<UClass>(Object);
		}		
	}

	WidgetNodes = GetWidgetsAttachedToScreen();

	LoadWidget();

	const FText SelectedText = (WidgetNodes.IsValidIndex(SelectedIndex)) ? WidgetNodes[SelectedIndex]->DisplayName() : FText::FromName(EmptyEntry);

	// create struct header
	HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
			[
			SAssignNew(Box, SComboBox<TSharedRef<FWidgetNode>>)
				.ToolTipText(PropertyHandle->GetToolTipText())
				.OptionsSource(&WidgetNodes)
				.OnGenerateWidget(this, &FWidgetSelectorCustomization::OnGenerateWidget)
				.OnSelectionChanged(this, &FWidgetSelectorCustomization::OnWidgetSelectionChanged)
				.Content()
				[
				SAssignNew(SelectedWidgetName, STextBlock)
					.Text(SelectedText)
					.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
				]
			];
}

void FWidgetSelectorCustomization::OnWidgetSelectionChanged(TSharedPtr<FWidgetNode> Node, ESelectInfo::Type Type)
{
	SelectedIndex = WidgetNodes.Find(Node.ToSharedRef());
	FText NameText = FText::FromName(EmptyEntry);

	if (SelectedIndex != INDEX_NONE)
	{
		NameText = Node->DisplayName();
	}

	SelectedWidgetName.Get()->SetText(NameText);

	SaveWidget(Node.ToSharedRef());
}

TSharedRef<SWidget> FWidgetSelectorCustomization::OnGenerateWidget(TSharedRef<FWidgetNode> InItem)
{
	return SNew(STextBlock)
		.Text(InItem->DisplayName())
		.Font(IPropertyTypeCustomizationUtils::GetRegularFont());
}

void FWidgetSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{

}

TArray<TSharedRef<FWidgetNode>> FWidgetSelectorCustomization::GetWidgetsAttachedToScreen()
{
	TArray<TSharedRef<FWidgetNode>> RetVal;
	const UWidgetTree* Tree = nullptr;


	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		// for cases where the selector is a uproperty of the class
		if (UWidgetBlueprintGeneratedClass* BPClass = Cast<UWidgetBlueprintGeneratedClass>(Screen.GetObject()->GetClass()))
		{
			Tree = BPClass->GetWidgetTreeArchetype();
		}
		else if(UUserWidget* AsUserWidget = Cast<UUserWidget>(Screen.GetObject()))// cases where the selector is a variable in blueprint
		{
			Tree = AsUserWidget->WidgetTree;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FWidgetSelectorCustomization::GetWidgetsAttachedToScreen: Could not get widget screen of %s"), *Screen.GetObject()->GetClass()->GetFName().ToString());
		}
	}

	TArray<FName> StartingPath;
	TraverseWidgetTree(Tree, RetVal, StartingPath);
	return RetVal;
}

void FWidgetSelectorCustomization::TraverseWidgetTree(const UWidgetTree* Tree, TArray<TSharedRef<FWidgetNode>>& RetVal, TArray<FName> Path)
{
	TArray<const UUserWidget*> Traversals;

	if (IsValid(Tree))
	{
		TArray<UWidget*> Children;
		Tree->GetAllWidgets(Children);

		for (const UWidget* Child : Children)
		{
			if (FilterEntry(Child))
			{
				TSharedRef<FWidgetNode> AsNode = MakeShared<FWidgetNode>(Child->GetFName());
				AsNode->Class = Child->GetClass();
				AsNode->Path = Path;
				RetVal.Emplace(AsNode);
			}

			// we have another tree to traverse
			if (const UUserWidget* AsUserWidget = Cast<UUserWidget>(Child))
			{
				Traversals.Emplace(AsUserWidget);
			}
		}
	}

	// traverse any userwidgets we found since they have their own widgettree. 
	for (const UUserWidget* UserWidget : Traversals)
	{
		if (UWidgetBlueprintGeneratedClass* BPClass = Cast<UWidgetBlueprintGeneratedClass>(UserWidget->GetClass()))
		{
			TArray<FName> AsPath = Path;
			AsPath.Emplace(UserWidget->GetFName());
			TraverseWidgetTree(BPClass->GetWidgetTreeArchetype(), RetVal, AsPath);
		}
	}
}

void FWidgetSelectorCustomization::SaveWidget(TSharedRef<FWidgetNode> SelectedNode)
{
	TSharedPtr<IPropertyHandle> WidgetName = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWidgetSelector, WidgetName));
	TSharedPtr<IPropertyHandle> WidgetPath = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWidgetSelector, WidgetPath));


	if (WidgetName && WidgetName->IsValidHandle())
	{
		ensure(WidgetName->SetValue(SelectedNode->WidgetName) == FPropertyAccess::Success);
	}

	if (WidgetPath && WidgetPath->IsValidHandle())
	{
		TSharedPtr<IPropertyHandleArray> WidgetPathArray = WidgetPath->AsArray();
		ensure(WidgetPathArray->EmptyArray() == FPropertyAccess::Success);

		for (int32 i=0; i<SelectedNode->Path.Num(); ++i)
		{
			ensure(WidgetPathArray->AddItem() == FPropertyAccess::Success);
			TSharedRef<IPropertyHandle> IndexProperty = WidgetPathArray->GetElement(i);

			if (IndexProperty->IsValidHandle())
			{
				ensure(IndexProperty->SetValue(SelectedNode->Path[i].ToString()) == FPropertyAccess::Success);
			}
		}
	}
}

void FWidgetSelectorCustomization::LoadWidget()
{
	TSharedPtr<IPropertyHandle> WidgetName = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWidgetSelector, WidgetName));
	TSharedPtr<IPropertyHandle> WidgetPath = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWidgetSelector, WidgetPath));

	FName Name;
	TArray<FName> Path;

	if (WidgetName)
	{
		ensure(WidgetName->GetValue(Name) == FPropertyAccess::Success);
	}

	if (WidgetPath)
	{
		TSharedPtr<IPropertyHandleArray> WidgetPathArray = WidgetPath->AsArray();

		uint32 PathCount;
		WidgetPathArray->GetNumElements(PathCount);
		for (uint32 i = 0; i < PathCount; ++i)
		{
			TSharedRef<IPropertyHandle> Handle = WidgetPathArray->GetElement(i);
			if (Handle->IsValidHandle())
			{
				FString Value;

				ensure(Handle->GetValue(Value) == FPropertyAccess::Success);
				if (!Value.IsEmpty())
				{
					Path.Emplace(FName(*Value));
				}
			}
		}
	}

	const FWidgetNode LoadedNode(Name, Path);
	for (int32 i=0; i<WidgetNodes.Num(); ++i)
	{
		if (WidgetNodes[i].Get() == LoadedNode)
		{
			SelectedIndex = i;
			break;
		}
	}
}


