// Copyright (c) Joel Gonzales


#include "UI/Screens/UICS/ViewWidgetInterface.h"

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
	bSelected = false;
	bFocused = false;
	SetEntry_Internal(nullptr);
}


bool IViewWidgetInterface::IsFocused_Implementation() const
{ 
	return bFocused; 
}

bool IViewWidgetInterface::IsSelected_Implementation() const
{ 
	return bSelected; 
}

bool IViewWidgetInterface::IsHovered_Implementation() const
{
	return bHovered;
}

void IViewWidgetInterface::SetHovered_Implementation(bool bInHovered)
{
	if (bHovered != bInHovered)
	{
		bHovered = bInHovered;
		FViewEvent& HoverDelegate = GetOnHoverChange();
		if (HoverDelegate.IsBound())
		{
			HoverDelegate.Broadcast(Cast<UObject>(this), bHovered);
		}
	}
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

void IViewWidgetInterface::SetFocus_Implementation(bool bInFocused)
{ 
	if (bInFocused != bFocused)
	{
		bFocused = bInFocused;
		FViewEvent& FocusDelegate = GetOnFocusChange();
		if (FocusDelegate.IsBound())
		{
			FocusDelegate.Broadcast(Cast<UObject>(this), bFocused);
		}
	}
}

void IViewWidgetInterface::SetSelected_Implementation(bool bInSelected)
{ 
	if (bSelected != bInSelected)
	{
		bSelected = bInSelected;
		FViewEvent& SelectionDelegate = GetOnSelectionChange();
		if (SelectionDelegate.IsBound())
		{
			SelectionDelegate.Broadcast(Cast<UObject>(this), bSelected);
		}
	}
}