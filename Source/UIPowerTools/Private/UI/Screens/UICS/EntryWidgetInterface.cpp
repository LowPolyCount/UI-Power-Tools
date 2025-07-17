// Copyright (c) Joel Gonzales


#include "UI/Screens/UICS/EntryWidgetInterface.h"

void IEntryWidgetInterface::SetEntryData_Implementation(int32 InIndex, UObject* InEntry)
{
	UObject* ThisAsUObject = Cast<UObject>(this);
	Index = InIndex;
	SetEntry_Internal(InEntry);
	Execute_OnEntryDataSet(ThisAsUObject, Index, GetEntry_Internal());
	Execute_Populate(ThisAsUObject, InEntry);
}

UObject* IEntryWidgetInterface::GetEntryData_Implementation() const
{ 
	return GetEntry_Internal();
}

void IEntryWidgetInterface::Reset_Implementation()
{
	Execute_OnReset(Cast<UObject>(this));

	// clear the data
	Index = INDEX_NONE;
	bSelected = false;
	bFocused = false;
	SetEntry_Internal(nullptr);
}


bool IEntryWidgetInterface::IsFocused_Implementation() const
{ 
	return bFocused; 
}

bool IEntryWidgetInterface::IsSelected_Implementation() const
{ 
	return bSelected; 
}

bool IEntryWidgetInterface::IsHovered_Implementation() const
{
	return bHovered;
}

void IEntryWidgetInterface::SetHovered_Implementation(bool bInHovered)
{
	bHovered = bInHovered;
}

void IEntryWidgetInterface::ForceInputAction_Implementation()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}

}

void IEntryWidgetInterface::OnInputAction_Implementation()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}

void IEntryWidgetInterface::SetFocus_Implementation(bool bInFocused)
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

void IEntryWidgetInterface::SetSelected_Implementation(bool bInSelected)
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