// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "ViewCommonUserWidget.generated.h"

/**
 * A CommonUserWidget that is usable by the View Screen Component by implementing IViewWidgetInterface
 */
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewCommonUserWidget : public UCommonUserWidget, public IViewWidgetInterface
{
	GENERATED_BODY()

public:
	VIEW_USERWIDGET_BOILERPLATE()
};
