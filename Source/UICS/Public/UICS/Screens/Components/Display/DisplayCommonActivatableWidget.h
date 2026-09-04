// Copyright 2025 Joel Gonzales

#pragma once

#include "CommonActivatableWidget.h"
#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "DisplayCommonActivatableWidget.generated.h"

/**
 * Activatable Widget that can be used as an Display widget with the Display Screen Component
 */
UCLASS(Abstract)
class UICS_API UDisplayCommonActivatableWidget : public UCommonActivatableWidget, public IDisplayWidgetInterface
{
	GENERATED_BODY()

public:
	DISPLAY_USERWIDGET_BOILERPLATE()

};
