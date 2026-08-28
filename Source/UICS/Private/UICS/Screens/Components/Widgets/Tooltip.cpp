// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Widgets/Tooltip.h"
#include "UICS/Screens/Components/Display/DisplayScreenComponent.h"
#include "UICS/Utility/UIPTStatics.h"

UTooltip::UTooltip()
	:Super()
{
	// we want tooltips to be hidden by default
	SetVisibility(HideRule);
}

void UTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	for (const FViewComponentSelector& ViewSelector : ViewsToListenTo)
	{
		StartListeningToView(ViewSelector);
	}
}

void UTooltip::NativeDestruct()
{
	for (const FViewComponentSelector& ViewSelector : ViewsToListenTo)
	{
		StopListeningToView(ViewSelector);
	}

	Super::NativeDestruct();
}

void UTooltip::StartListeningToView(const FViewComponentSelector& ViewSelector)
{
	if (ViewSelector.IsValid())
	{
		if (UDisplayScreenComponent* ViewComponent = UUIPTStatics::GetScreenComponentFromSelector<UDisplayScreenComponent>(this, ViewSelector))
		{
			ViewComponent->OnInputAction.AddUniqueDynamic(this, &UTooltip::HandleOnAction);
			ViewComponent->OnFocusChange.AddUniqueDynamic(this, &UTooltip::HandleOnFocusChange);
			ViewComponent->OnSelectionChange.AddUniqueDynamic(this, &UTooltip::HandleOnSelectionChange);
			ViewComponent->OnHoverChange.AddUniqueDynamic(this, &UTooltip::HandleOnHoverChange);
		}
	}
}

void UTooltip::StopListeningToView(const FViewComponentSelector& ViewSelector)
{
	if (ViewSelector.IsValid())
	{
		if (UDisplayScreenComponent* ViewComponent = UUIPTStatics::GetScreenComponentFromSelector<UDisplayScreenComponent>(this, ViewSelector))
		{
			ViewComponent->OnInputAction.RemoveDynamic(this, &UTooltip::HandleOnAction);
			ViewComponent->OnFocusChange.RemoveDynamic(this, &UTooltip::HandleOnFocusChange);
			ViewComponent->OnSelectionChange.RemoveDynamic(this, &UTooltip::HandleOnSelectionChange);
			ViewComponent->OnHoverChange.RemoveDynamic(this, &UTooltip::HandleOnHoverChange);
		}
	}
}

void UTooltip::HandleOnAction(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget)
{
	BP_HandleOnAction(Component, Widget);
}

void UTooltip::HandleOnSelectionChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	BP_HandleOnSelectionChange(Component, Widget, bGained);
}

void UTooltip::HandleOnFocusChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	if (bGained && bShowOnFocusGain)
	{
		ShowOrHideTooltip(Widget, bGained);
	}
	else if (!bGained && bHideOnFocusLoss)
	{
		ShowOrHideTooltip(Widget, bGained);
	}

	BP_HandleOnFocusChange(Component, Widget, bGained);
}

void UTooltip::HandleOnHoverChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	if (bGained && bShowOnHoverGain)
	{
		ShowOrHideTooltip(Widget, bGained);
	}
	else if (!bGained && bHideOnHoverLoss)
	{
		ShowOrHideTooltip(Widget, bGained);
	}

	BP_HandleOnHoverChange(Component, Widget, bGained);
}

void UTooltip::Populate(UObject* Data)
{
	if (ObservedEntryData)
	{
		Reset();
	}
	SetObservedEntryData(Data);
	BP_Populate(Data);
}

void UTooltip::Reset()
{
	BP_Reset();
}

void UTooltip::ShowOrHideTooltip(const TScriptInterface<IDisplayWidgetInterface>& Widget, const bool bGained)
{
	SetVisibility((bGained) ? ShowRule : HideRule);
	if (bGained)
	{
		Populate(Widget->Execute_GetEntryData(Widget.GetObject()));
	}
	else
	{
		Reset();
	}
}

void UTooltip::SetObservedEntryData(UObject* InData)
{
	ObservedEntryData = InData;
}