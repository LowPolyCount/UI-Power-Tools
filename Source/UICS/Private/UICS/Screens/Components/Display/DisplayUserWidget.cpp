// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information


#include "UICS/Screens/Components/Display/DisplayUserWidget.h"

FReply UDisplayUserWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	const FReply RetVal = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	SetFocus_Internal(true);
	return RetVal;
}

void UDisplayUserWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	SetFocus_Internal(false);
}

void UDisplayUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetHovered_Internal(true);
}

void UDisplayUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetHovered_Internal(false);
}
