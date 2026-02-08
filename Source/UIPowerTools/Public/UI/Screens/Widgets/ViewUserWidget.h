// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "ViewUserWidget.generated.h"

// a UserWidget that can be used with View Screen Component
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewUserWidget : public UUserWidget, public IViewWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

public:
	VIEW_USERWIDGET_BOILERPLATE();
};
