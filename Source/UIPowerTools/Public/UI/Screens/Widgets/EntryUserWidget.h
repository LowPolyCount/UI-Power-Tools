// Copyright 2025 Joel Gonzales

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Screens/UICS/EntryWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "EntryUserWidget.generated.h"

// a UserWidget that implements EntryWidget Interface
UCLASS(Abstract)
class UIPOWERTOOLS_API UEntryUserWidget : public UUserWidget, public IEntryWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

	VIEW_WIDGET_BOILERPLATE();
};
