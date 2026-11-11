// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information


#include "UICS/Screens/Tools/WidgetSelector.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "UICS/Screens/UICSScreen.h"
#include "UICS/Screens/Components/IUICSAccessor.h"
#include "Blueprint/WidgetTree.h"



FSelector::FSelector(const UClass* Filter)
#if WITH_EDITORONLY_DATA
	: ClassFilter(Filter)
#endif
{

}

FWidgetSelector::FWidgetSelector(const UClass* Filter)
#if WITH_EDITORONLY_DATA
	: FSelector(Filter)
#endif
{

}

FPanelWidgetSelector::FPanelWidgetSelector()
	:FWidgetSelector(UPanelWidget::StaticClass())
{

}

UWidget* FWidgetSelector::GetSelectedWidget(UWidget* WidgetOwner) const
{
	return GetSelectedWidgetInternal(WidgetOwner, false);
}

bool FWidgetSelector::IsValid() const
{
	return !WidgetName.IsNone();
}

UWidget* FWidgetSelector::GetSelectedWidgetInternal(UWidget* WidgetOwner, bool bInBlueprint) const
{
	UWidget* RetVal = nullptr;
	TScriptInterface<IUICSScreenAccessor> AsScreen(WidgetOwner);
	if (!AsScreen && WidgetOwner)
	{
		UObjectBaseUtility* Outer = WidgetOwner->GetImplementingOuterObject(UUICSScreenAccessor::StaticClass());
		AsScreen = TScriptInterface<IUICSScreenAccessor>(Cast<UObject>(Outer));
		if (!AsScreen)
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get owning Screen during GetWidgetAtEditorTime() functions may not work"));
		}
	}

	const UWidgetTree* Tree = nullptr;

	if (bInBlueprint)
	{
		if (const UWidgetBlueprintGeneratedClass* BPClass = Cast<UWidgetBlueprintGeneratedClass>(AsScreen.GetObject()->GetClass()))
		{
			Tree = BPClass->GetWidgetTreeArchetype();
		}
	}
	else if(UUserWidget * AsUWidget = Cast<UUserWidget>(AsScreen.GetObject()))
	{
		Tree = AsUWidget->WidgetTree;
	}

	if (Tree)
	{
		if (bInBlueprint)
		{
			for (const FName& Name : WidgetPath)
			{
				if (Tree)
				{
					const UWidget* FoundWidget = Tree->FindWidget(Name);
					if(const UWidgetBlueprintGeneratedClass* AsClass = Cast<UWidgetBlueprintGeneratedClass>(FoundWidget->GetClass()))
					{
						Tree = AsClass->GetWidgetTreeArchetype();
					}
				}
			}

			if (Tree)
			{
				RetVal = Tree->FindWidget(WidgetName);
			}
		}
		else
		{
			// travel down a series of userwidgets if needed.
			for (const FName& Name : WidgetPath)
			{
				if (Tree)
				{
					if (UUserWidget* Found = Cast<UUserWidget>(Tree->FindWidget(Name)))
					{
						Tree = Found->WidgetTree;
					}
				}
			}
		}

		if (Tree)
		{
			RetVal = Tree->FindWidget(WidgetName);
		}

	}
	return RetVal;
}