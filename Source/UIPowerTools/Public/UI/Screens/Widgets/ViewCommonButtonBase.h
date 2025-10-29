// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "ViewCommonButtonBase.generated.h"

// a Common Button Base that can be used as an view widget
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewCommonButtonBase : public UCommonButtonBase, public IViewWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

public:
	VIEW_BUTTON_AND_WIDGET_BOILERPLATE()
};
