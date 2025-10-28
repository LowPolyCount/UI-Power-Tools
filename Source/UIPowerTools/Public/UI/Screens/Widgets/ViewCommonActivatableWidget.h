// Copyright 2025 Joel Gonzales

#pragma once

#include "CommonActivatableWidget.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "ViewCommonActivatableWidget.generated.h"

/**
 * Activatable Widget that can be used with View Screen Components
 */
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewCommonActivatableWidget : public UCommonActivatableWidget, public IViewWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

public:
	VIEW_WIDGET_BOILERPLATE()

};
