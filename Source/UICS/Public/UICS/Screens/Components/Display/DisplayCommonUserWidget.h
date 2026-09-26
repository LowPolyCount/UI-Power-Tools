// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "DisplayCommonUserWidget.generated.h"

/**
 * A CommonUserWidget that can be used as an Display widget with the Display Screen Component
 */
UCLASS(Abstract)
class UICS_API UDisplayCommonUserWidget : public UCommonUserWidget, public IDisplayWidgetInterface
{
	GENERATED_BODY()

public:
	DISPLAY_USERWIDGET_BOILERPLATE()
};
