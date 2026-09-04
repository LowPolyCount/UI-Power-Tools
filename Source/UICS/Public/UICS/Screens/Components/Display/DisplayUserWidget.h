// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "DisplayUserWidget.generated.h"

// a UserWidget that can be used as an Display widget with the Display Screen Component
UCLASS(Abstract)
class UICS_API UDisplayUserWidget : public UUserWidget, public IDisplayWidgetInterface
{
	GENERATED_BODY()

public:
	DISPLAY_USERWIDGET_BOILERPLATE();
};
