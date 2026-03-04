// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "ViewCommonButtonBase.generated.h"

// a Common Button Base that can be used as an view widget
UCLASS(Abstract)
class UIPOWERTOOLS_API UViewCommonButtonBase : public UCommonButtonBase, public IViewWidgetInterface
{
	GENERATED_BODY()

public:
	VIEW_COMMONBUTTON_BOILERPLATE();
};
