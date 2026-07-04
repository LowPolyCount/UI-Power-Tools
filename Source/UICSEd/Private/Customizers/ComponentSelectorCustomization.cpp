// Copyright (c) Joel Gonzales

#include "Customizers/ComponentSelectorCustomization.h"
#include "UI/Screens/UICS/ScreenComponent.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "UI/Screens/UICSScreen.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

using namespace Customization;

static const FString EmptyText = TEXT("<None>");
static const FString Delimiter = TEXT("/");

FComponentNode::FComponentNode(const UScreenComponent* InComponent, int32 InIndex)
	:SelectedComponent(InComponent)
	,Index(InIndex)
{
}

bool FComponentNode::operator==(const FComponentNode& rhs) const
{
	bool bRetVal = false;
	if (SelectedComponent)
	{
		bRetVal = (rhs.SelectedComponent != nullptr) ? SelectedComponent->GetGuid() == rhs.SelectedComponent->GetGuid() : false;
	}
	return bRetVal;
}

FString FComponentNode::DisplayName() const
{
	FString RetVal;
	
	RetVal = TEXT(" [") + FString::FromInt(Index);
	RetVal += TEXT("] ");

	RetVal += (SelectedComponent) ? SelectedComponent->GetDisplayName() : FString();
	return RetVal;
}

FString FComponentNode::DisplayNameVerbose() const
{
	FString RetVal;

	RetVal = TEXT(" [") + FString::FromInt(Index);
	RetVal += TEXT("] ");

	RetVal += (SelectedComponent) ? SelectedComponent->GetDisplayNameVerbose() : FString();
	return RetVal;
}

TSharedRef<IPropertyTypeCustomization> FComponentSelectorCustomization::MakeInstance()
{
	return MakeShared<FComponentSelectorCustomization>();
}

void FComponentSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	FBaseSelectorCustomization::CustomizeHeader(PropertyHandle, HeaderRow, CustomizationUtils);

	TSharedPtr<IPropertyHandle> ClassFilterProperty = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FComponentSelector, ClassFilter));
	if (ClassFilterProperty && ClassFilterProperty->IsValidHandle())
	{
		UObject* Object;
		ClassFilterProperty->GetValue(Object);
		if (Object)
		{
			ClassFilter = Cast<UClass>(Object);
		}
	}

	ComponentNodes = GetComponentsOnScreen();

	LoadScreenComponent();

	const FText SelectedText = FText::FromString((ComponentNodes.IsValidIndex(SelectedIndex)) ? ComponentNodes[SelectedIndex]->DisplayNameVerbose() : EmptyText);

	// create struct header
	HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(Box, SComboBox<TSharedRef<FComponentNode>>)
				.ToolTipText(PropertyHandle->GetToolTipText())
				.OptionsSource(&ComponentNodes)
				.OnGenerateWidget(this, &FComponentSelectorCustomization::OnGenerateWidget)
				.OnSelectionChanged(this, &FComponentSelectorCustomization::OnWidgetSelectionChanged)
				//.InitiallySelectedItem((ComponentNodes.IsValidIndex(SelectedIndex)) ? ComponentNodes[SelectedIndex].ToSharedPtr() : nullptr)
				.Content()
				[
					SAssignNew(SelectedWidgetName, STextBlock)
						.Text(SelectedText)
						.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
				]
		];
}

void FComponentSelectorCustomization::OnWidgetSelectionChanged(TSharedPtr<FComponentNode> Node, ESelectInfo::Type Type)
{
	SelectedIndex = ComponentNodes.Find(Node.ToSharedRef());
	FString NameText = EmptyText;

	if (SelectedIndex != INDEX_NONE)
	{
		NameText = Node->DisplayName();
	}

	SelectedWidgetName.Get()->SetText(FText::FromString(NameText));

	SaveScreenComponent(Node.ToSharedRef());
}

TSharedRef<SWidget> FComponentSelectorCustomization::OnGenerateWidget(TSharedRef<FComponentNode> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(InItem->DisplayNameVerbose()))
		.Font(IPropertyTypeCustomizationUtils::GetRegularFont());
}

void FComponentSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{

}

TArray<TSharedRef<FComponentNode>> FComponentSelectorCustomization::GetComponentsOnScreen()
{
	TArray<TSharedRef<FComponentNode>> RetVal;
	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		TSubclassOf<UScreenComponent> TClassFilter(ClassFilter);
		TArray<UScreenComponent*> AllComponents = Screen->GetAllScreenComponents_BP(TClassFilter);

		const int32 CompNum = AllComponents.Num();
		for (int32 i = 0; i < CompNum; ++i)
		{
			FComponentNode Node(AllComponents[i], i);
			RetVal.Emplace(MakeShared<FComponentNode>(Node));
		}
	}

	return RetVal;
}

void FComponentSelectorCustomization::SaveScreenComponent(TSharedRef<FComponentNode> SelectedNode)
{
	TSharedPtr<IPropertyHandle> ComponentGuid = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FComponentSelector, ComponentGuid));

	if((ComponentGuid && ComponentGuid->IsValidHandle()) && (SelectedNode->SelectedComponent))
	{ 
		const FString AsString = SelectedNode->SelectedComponent->GetGuid().ToString();
		ensure(ComponentGuid->SetValueFromFormattedString(AsString) == FPropertyAccess::Success);
	}
}

void FComponentSelectorCustomization::LoadScreenComponent()
{
	UScreenComponent* LoadedComponent = nullptr;
	FGuid SelectedGuid;
	TSharedPtr<IPropertyHandle> ComponentGuid = PropertyToCustomize->GetChildHandle(GET_MEMBER_NAME_CHECKED(FComponentSelector, ComponentGuid));
	
	//TSharedPtr<IPropertyHandle> ComponentGuid = PropertyToCustomize->GetChildHandle(GET_FUNCTION_NAME_CHECKED(FComponentSelector, GetComponentGuid));
	if (ComponentGuid && ComponentGuid->IsValidHandle())
	{
		FString AsString;
		ComponentGuid->GetValueAsFormattedString(AsString);
		SelectedGuid = FGuid(AsString);
	}

	
	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		LoadedComponent = Screen->GetScreenComponentFromGUID(SelectedGuid);
	}

	
	const FComponentNode LoadedNode(LoadedComponent, INDEX_NONE);
	for (int32 i = 0; i < ComponentNodes.Num(); ++i)
	{
		if (ComponentNodes[i].Get() == LoadedNode)
		{
			SelectedIndex = i;
			break;
		}
	}
}


