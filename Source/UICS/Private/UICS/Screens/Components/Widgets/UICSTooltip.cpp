// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Widgets/UICSTooltip.h"
#include "UICS/Screens/Components/Display/DisplayScreenComponent.h"
#include "UICS/Utility/UIPTStatics.h"

UUICSTooltip::UUICSTooltip()
	:Super()
{
	// we want tooltips to be hidden by default
	SetVisibility(HideRule);
}

void UUICSTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	for (const FDisplayComponentSelector& ViewSelector : LinkedDisplayScreenComponents)
	{
		StartListeningToView(ViewSelector);
	}
}

void UUICSTooltip::NativeDestruct()
{
	for (const FDisplayComponentSelector& ViewSelector : LinkedDisplayScreenComponents)
	{
		StopListeningToView(ViewSelector);
	}

	Super::NativeDestruct();
}

void UUICSTooltip::StartListeningToView(const FDisplayComponentSelector& ViewSelector)
{
	if (ViewSelector.IsValid())
	{
		if (UDisplayScreenComponent* ViewComponent = UUIPTStatics::GetScreenComponentFromSelector<UDisplayScreenComponent>(this, ViewSelector))
		{
			ViewComponent->OnInputAction.AddUniqueDynamic(this, &UUICSTooltip::HandleOnAction);
			ViewComponent->OnFocusChange.AddUniqueDynamic(this, &UUICSTooltip::HandleOnFocusChange);
			ViewComponent->OnSelectionChange.AddUniqueDynamic(this, &UUICSTooltip::HandleOnSelectionChange);
			ViewComponent->OnHoverChange.AddUniqueDynamic(this, &UUICSTooltip::HandleOnHoverChange);
			ViewComponent->OnAction.AddUniqueDynamic(this, &UUICSTooltip::HandleOnAction);
		}
	}
}

void UUICSTooltip::StopListeningToView(const FDisplayComponentSelector& ViewSelector)
{
	if (ViewSelector.IsValid())
	{
		if (UDisplayScreenComponent* ViewComponent = UUIPTStatics::GetScreenComponentFromSelector<UDisplayScreenComponent>(this, ViewSelector))
		{
			ViewComponent->OnInputAction.RemoveDynamic(this, &UUICSTooltip::HandleOnAction);
			ViewComponent->OnFocusChange.RemoveDynamic(this, &UUICSTooltip::HandleOnFocusChange);
			ViewComponent->OnSelectionChange.RemoveDynamic(this, &UUICSTooltip::HandleOnSelectionChange);
			ViewComponent->OnHoverChange.RemoveDynamic(this, &UUICSTooltip::HandleOnHoverChange);
			ViewComponent->OnAction.RemoveDynamic(this, &UUICSTooltip::HandleOnAction);
		}
	}
}

void UUICSTooltip::HandleOnAction(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget)
{
	BP_HandleOnAction(Component, Widget);
}

void UUICSTooltip::CheckForShowAndHide(const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained, bool bPopulateWhenGained, bool bHideWhenLost)
{
	if (bGained && bPopulateWhenGained)
	{
		ShowOrHideTooltip(Widget, bGained);
	}
	else if (!bGained && bHideWhenLost)
	{
		ShowOrHideTooltip(Widget, bGained);
	}
}

void UUICSTooltip::HandleOnSelectionChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	CheckForShowAndHide(Widget, bGained, bShowAndPopulateWhenWidgetGainsSelected, bHideWhenWidgetLosesSelected);
	BP_HandleOnSelectionChange(Component, Widget, bGained);
}

void UUICSTooltip::HandleOnFocusChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	if (bGained && bShowAndPopulateWhenWidgetGainsFocus)
	{
		ShowOrHideTooltip(Widget, bGained);
	}
	else if (!bGained && bHideWhenWidgetLosesFocus)
	{
		ShowOrHideTooltip(Widget, bGained);
	}

	BP_HandleOnFocusChange(Component, Widget, bGained);
}

void UUICSTooltip::HandleOnHoverChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	if (bGained && bShowAndPopulateWhenWidgetGainsHover)
	{
		ShowOrHideTooltip(Widget, bGained);
	}
	else if (!bGained && bHideWhenWidgetLosesHover)
	{
		ShowOrHideTooltip(Widget, bGained);
	}

	BP_HandleOnHoverChange(Component, Widget, bGained);
}

void UUICSTooltip::Populate(const TScriptInterface<IDisplayWidgetInterface> Widget, UObject* Data)
{
	if (ObservedEntryData)
	{
		Reset();
	}
	SetObservedEntryData(Data);
	BP_Populate(Widget, Data);
}

void UUICSTooltip::Reset()
{
	BP_Reset();
}

void UUICSTooltip::ShowOrHideTooltip(const TScriptInterface<IDisplayWidgetInterface>& Widget, const bool bGained)
{
	SetVisibility((bGained) ? ShowRule : HideRule);
	if (bGained)
	{
		Populate(Widget, Widget->Execute_GetEntryData(Widget.GetObject()));
	}
	else
	{
		Reset();
	}
}

void UUICSTooltip::SetObservedEntryData(UObject* InData)
{
	ObservedEntryData = InData;
}