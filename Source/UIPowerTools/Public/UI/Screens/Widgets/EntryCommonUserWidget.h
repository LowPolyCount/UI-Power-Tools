// Copyright 2025 Joel Gonzales

#pragma once

#include "CommonUserWidget.h"
#include "UI/Screens/UICS/EntryWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "EntryCommonUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UIPOWERTOOLS_API UEntryCommonUserWidget : public UCommonUserWidget, public IEntryWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

public:
	VIEW_WIDGET_BOILERPLATE()

};
