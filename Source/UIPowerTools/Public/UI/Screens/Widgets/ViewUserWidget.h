// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Screens/UICS/EntryWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "ViewUserWidget.generated.h"

// a UserWidget that implements ViewWidget Interface
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewUserWidget : public UUserWidget, public IEntryWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

public:
	VIEW_WIDGET_BOILERPLATE();
};
