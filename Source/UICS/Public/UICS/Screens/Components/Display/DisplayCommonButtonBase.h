// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "DisplayCommonButtonBase.generated.h"

// a Common Button Base that can be used as an Display widget with the Display Screen Component
UCLASS(Abstract)
class UICS_API UDisplayCommonButtonBase : public UCommonButtonBase, public IDisplayWidgetInterface
{
	GENERATED_BODY()

public:
	DISPLAY_COMMONBUTTON_BOILERPLATE();
};
