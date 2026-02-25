// Copyright 2025 Joel Gonzales

#pragma once

#include "CommonActivatableWidget.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "ViewCommonActivatableWidget.generated.h"

/**
 * Activatable Widget that can be used with View Screen Components
 */
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewCommonActivatableWidget : public UCommonActivatableWidget, public IViewWidgetInterface
{
	GENERATED_BODY()

public:
	VIEW_USERWIDGET_BOILERPLATE()

};
