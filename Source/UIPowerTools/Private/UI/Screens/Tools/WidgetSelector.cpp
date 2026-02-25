// Copyright 2025 Joel Gonzales


#include "UI/Screens/Tools/WidgetSelector.h"
#include "UI/Screens/UICSScreen.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
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


UWidget* FWidgetSelector::GetWidget(UWidget* WidgetOwner) const
{
	UWidget* RetVal = nullptr;
	TScriptInterface<IUICSScreenAccessor> AsScreen(WidgetOwner);
	if (!AsScreen && WidgetOwner)
	{
		UObjectBaseUtility* Outer = WidgetOwner->GetImplementingOuterObject(UUICSScreenAccessor::StaticClass());
		AsScreen = TScriptInterface<IUICSScreenAccessor>(Cast<UObject>(Outer));
		if (!AsScreen)
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get owning Screen during GetWidget() functions will not work"));
		}
	}

	if (UUserWidget* AsUWidget = Cast<UUserWidget>(AsScreen.GetObject()))
	{
		if (const UWidgetTree* Tree = AsUWidget->WidgetTree)
		{
			for (const FName& Name : WidgetPath)
			{
				if (UUserWidget* Found = Cast<UUserWidget>(Tree->FindWidget(Name)))
				{
					Tree = Found->WidgetTree;
				}
			}

			RetVal = Tree->FindWidget(WidgetName);
		}
	}
	return RetVal;
}

bool FWidgetSelector::IsValid() const
{
	return !WidgetName.IsNone();
}


FPanelWidgetSelector::FPanelWidgetSelector()
	:FWidgetSelector(UPanelWidget::StaticClass())
{

}