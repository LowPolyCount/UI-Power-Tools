// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information

#pragma once

#include "CommonActivatableWidget.h"
#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "DisplayCommonActivatableWidget.generated.h"

/**
 * Activatable Widget that can be used as an Display widget with the Display Screen Component
 */
UCLASS(Abstract)
class UICS_API UDisplayCommonActivatableWidget : public UCommonActivatableWidget, public IDisplayWidgetInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = DisplayWidget)
	FViewAction OnInputAction;
	UPROPERTY(BlueprintAssignable, Category = DisplayWidget)
	FViewEvent OnSelectionChanged;
	UPROPERTY(BlueprintAssignable, Category = DisplayWidget)
	FViewEvent OnFocusChanged;
	UPROPERTY(BlueprintAssignable, Category = DisplayWidget)
	FViewEvent OnHoverChange;
	virtual FViewAction& GetOnAction() { return OnInputAction; }
	virtual FViewEvent& GetOnSelectionChange() { return OnSelectionChanged; }
	virtual FViewEvent& GetOnFocusChange() { return OnFocusChanged; }
	virtual FViewEvent& GetOnHoverChange() { return OnHoverChange; }

	// userwidget functions 
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
