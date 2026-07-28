// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UICS/Screens/Components/Display/ViewWidgetInterface.h"
#include "ViewUserWidget.generated.h"

// a UserWidget that can be used with View Screen Component
UCLASS(Abstract)
class UICS_API UViewUserWidget : public UUserWidget, public IViewWidgetInterface
{
	GENERATED_BODY()

public:
	VIEW_USERWIDGET_BOILERPLATE();
};
