// Copyright (c) Joel Gonzales


#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"

void IViewWidgetInterface::SetOwningViewScreenComponent(UViewScreenComponent* InOwningComponent)
{ 
	ManagingViewScreenComponent = TWeakObjectPtr<UViewScreenComponent>(InOwningComponent);
}

UViewScreenComponent* IViewWidgetInterface::GetOwningViewScreenComponent_Implementation() const
{
	return (ManagingViewScreenComponent.IsValid()) ? ManagingViewScreenComponent.Pin().Get() : nullptr;
}

void IViewWidgetInterface::SetEntryData_Implementation(int32 InIndex, UObject* InEntry)
{
	UObject* ThisAsUObject = Cast<UObject>(this);
	Index = InIndex;
	SetEntry_Internal(InEntry);
	Execute_OnEntryDataSet(ThisAsUObject, Index, GetEntry_Internal());
	Execute_Populate(ThisAsUObject, InEntry);
}

UObject* IViewWidgetInterface::GetEntryData_Implementation() const
{ 
	return GetEntry_Internal();
}

void IViewWidgetInterface::Reset_Implementation()
{
	Execute_OnReset(Cast<UObject>(this));

	// clear the data
	Index = INDEX_NONE;
	SetEntry_Internal(nullptr);
}


bool IViewWidgetInterface::IsFocused_Implementation() const
{ 
	return false; 
}

bool IViewWidgetInterface::IsSelected_Implementation() const
{ 
	return false; 
}

bool IViewWidgetInterface::IsHovered_Implementation() const
{
	return false;
}



void IViewWidgetInterface::ForceInputAction_Implementation()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}

void IViewWidgetInterface::OnInputAction_Implementation()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}

void IViewWidgetInterface::SetHovered_Implementation(bool bInHovered)
{
	SetHovered_Internal(bInHovered);
}

void IViewWidgetInterface::SetFocus_Implementation(bool bInFocused)
{ 
	SetFocus_Internal(bInFocused);
}

void IViewWidgetInterface::SetSelected_Implementation(bool bInSelected)
{ 
	SetSelected_Internal(bInSelected);
}

void IViewWidgetInterface::SetFocus_Internal(bool bInFocused)
{
	FViewEvent& FocusDelegate = GetOnFocusChange();
	if (FocusDelegate.IsBound())
	{
		FocusDelegate.Broadcast(Cast<UObject>(this), bInFocused);
	}
}

void IViewWidgetInterface::SetHovered_Internal(bool bInHovered)
{
	FViewEvent& HoveredDelegate = GetOnHoverChange();
	if (HoveredDelegate.IsBound())
	{
		HoveredDelegate.Broadcast(Cast<UObject>(this), bInHovered);
	}
}

void IViewWidgetInterface::SetSelected_Internal(bool bInSelected)
{
	FViewEvent& SelectionDelegate = GetOnSelectionChange();
	if (SelectionDelegate.IsBound())
	{
		SelectionDelegate.Broadcast(Cast<UObject>(this), bInSelected);
	}
}

void IViewWidgetInterface::SetInputAction_Internal()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}