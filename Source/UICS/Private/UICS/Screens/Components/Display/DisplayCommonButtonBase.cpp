// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information


#include "UICS/Screens/Components/Display/DisplayCommonButtonBase.h"

void UDisplayCommonButtonBase::HandleFocusReceived()
{
		Super::HandleFocusReceived();
		SetFocus_Internal(true);
}

void UDisplayCommonButtonBase::HandleFocusLost()
{
	Super::HandleFocusLost();
	SetFocus_Internal(false);
}

void UDisplayCommonButtonBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetHovered_Internal(true);
}

void UDisplayCommonButtonBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetHovered_Internal(false);
}

// button specific functions
void UDisplayCommonButtonBase::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);
	SetSelected_Internal(true);
}

void UDisplayCommonButtonBase::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
	SetSelected_Internal(false);
}

void UDisplayCommonButtonBase::NativeOnClicked()
{
	Super::NativeOnClicked();
	SetInputAction_Internal();
}


