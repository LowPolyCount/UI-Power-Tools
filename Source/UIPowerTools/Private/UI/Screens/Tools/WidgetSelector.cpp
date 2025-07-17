// Copyright 2025 Joel Gonzales


#include "UI/Screens/Tools/WidgetSelector.h"
#include "UI/Screens/Screen.h"
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
	UScreen* AsScreen = Cast<UScreen>(WidgetOwner);
	if (!AsScreen)
	{
		AsScreen = WidgetOwner->GetTypedOuter<UScreen>();
	}

	if (AsScreen)
	{
		if (const UWidgetTree* Tree = AsScreen->WidgetTree)
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