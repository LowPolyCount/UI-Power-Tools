// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "ViewUserWidget.generated.h"

// a UserWidget that can be used with View Screen Component
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewUserWidget : public UUserWidget, public IViewWidgetInterface
{
	GENERATED_BODY()

public:
	VIEW_USERWIDGET_BOILERPLATE();
};
