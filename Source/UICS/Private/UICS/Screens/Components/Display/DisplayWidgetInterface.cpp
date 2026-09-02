// Copyright (c) Joel Gonzales


#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "UICS/Screens/Components/Display/DisplayScreenComponent.h"
#include "UICS/Screens/Components/Action/ActionScreenComponent.h"

void IDisplayWidgetInterface::SetOwningDisplayScreenComponent(UDisplayScreenComponent* InOwningComponent)
{
	OwningDisplayScreenComponent = TWeakObjectPtr<UDisplayScreenComponent>(InOwningComponent);
}

UDisplayScreenComponent* IDisplayWidgetInterface::GetOwningDisplayScreenComponent_Implementation() const
{
	return (OwningDisplayScreenComponent.IsValid()) ? OwningDisplayScreenComponent.Pin().Get() : nullptr;
}

bool IDisplayWidgetInterface::HasLinkedActionScreenComponent_Implementation() const
{
	return Execute_GetLinkedActionScreenComponent(Cast<UObject>(this)) != nullptr;
}

UActionScreenComponent* IDisplayWidgetInterface::GetLinkedActionScreenComponent_Implementation() const
{
	UActionScreenComponent* RetVal = nullptr;
	if (UDisplayScreenComponent* VSC = Execute_GetOwningDisplayScreenComponent(Cast<UObject>(this)))
	{
		RetVal = VSC->GetLinkedActionComponent();
	}

	return RetVal;
}

bool IDisplayWidgetInterface::CanExecuteAction_Implementation()
{
	bool bRetVal = false;	// in the case of an invalid ASC, assume you can't execute the action

	if (UActionScreenComponent* ASC = Execute_GetLinkedActionScreenComponent(Cast<UObject>(this)))
	{
		bRetVal = ASC->CanExecuteAction(this->Execute_GetEntryData(Cast<UObject>(this)));
	}
	return bRetVal;
}

void IDisplayWidgetInterface::SetEntryData_Implementation(int32 InIndex, UObject* InEntry)
{
	UObject* ThisAsUObject = Cast<UObject>(this);
	Index = InIndex;
	SetEntry_Internal(InEntry);
	Execute_OnEntryDataSet(ThisAsUObject, Index, GetEntry_Internal());
	Execute_Populate(ThisAsUObject, InEntry);
}

UObject* IDisplayWidgetInterface::GetEntryData_Implementation() const
{ 
	return GetEntry_Internal();
}

void IDisplayWidgetInterface::Reset_Implementation()
{
	Execute_OnReset(Cast<UObject>(this));

	// clear the data
	Index = INDEX_NONE;
	SetEntry_Internal(nullptr);
}


bool IDisplayWidgetInterface::IsFocused_Implementation() const
{ 
	return false; 
}

bool IDisplayWidgetInterface::IsSelected_Implementation() const
{ 
	return false; 
}

bool IDisplayWidgetInterface::IsHovered_Implementation() const
{
	return false;
}



void IDisplayWidgetInterface::ForceInputAction_Implementation()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}

void IDisplayWidgetInterface::OnInputAction_Implementation()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}

void IDisplayWidgetInterface::SetHovered_Implementation(bool bInHovered)
{
	SetHovered_Internal(bInHovered);
}

void IDisplayWidgetInterface::SetFocus_Implementation(bool bInFocused)
{ 
	SetFocus_Internal(bInFocused);
}

void IDisplayWidgetInterface::SetSelected_Implementation(bool bInSelected)
{ 
	SetSelected_Internal(bInSelected);
}

void IDisplayWidgetInterface::SetFocus_Internal(bool bInFocused)
{
	FViewEvent& FocusDelegate = GetOnFocusChange();
	if (FocusDelegate.IsBound())
	{
		FocusDelegate.Broadcast(Cast<UObject>(this), bInFocused);
	}
}

void IDisplayWidgetInterface::SetHovered_Internal(bool bInHovered)
{
	FViewEvent& HoveredDelegate = GetOnHoverChange();
	if (HoveredDelegate.IsBound())
	{
		HoveredDelegate.Broadcast(Cast<UObject>(this), bInHovered);
	}
}

void IDisplayWidgetInterface::SetSelected_Internal(bool bInSelected)
{
	FViewEvent& SelectionDelegate = GetOnSelectionChange();
	if (SelectionDelegate.IsBound())
	{
		SelectionDelegate.Broadcast(Cast<UObject>(this), bInSelected);
	}
}

void IDisplayWidgetInterface::SetInputAction_Internal()
{
	FViewAction& ActionDelegate = GetOnAction();
	if (ActionDelegate.IsBound())
	{
		ActionDelegate.Broadcast(Cast<UObject>(this));
	}
}

FGameplayTag IDisplayWidgetInterface::GetLastActionResult_Implementation() const
{
	FGameplayTag RetVal = UICS_ACTION_NoActionComponent;
	if (const UActionScreenComponent* ASC = Execute_GetLinkedActionScreenComponent(Cast<UObject>(this)))
	{
		RetVal = ASC->GetLastActionResult();
	}
	return RetVal;
}

bool IDisplayWidgetInterface::HasTextAssociatedWithLastActionResultTag_Implementation() const
{
	bool bRetVal = false;
	if (const UActionScreenComponent* ASC = Execute_GetLinkedActionScreenComponent(Cast<UObject>(this)))
	{
		bRetVal = ASC->HasTextAssociatedWithLastActionResultTag();
	}
	return bRetVal;
}


FText IDisplayWidgetInterface::GetTextAssociatedWithLastActionResultTag_Implementation() const
{
	FText RetVal;
	if (const UActionScreenComponent* ASC = Execute_GetLinkedActionScreenComponent(Cast<UObject>(this)))
	{
		RetVal = ASC->GetTextAssociatedWithLastActionResultTag();
	}
	return RetVal;
}

void IDisplayWidgetInterface::Release()
{
	Entry.Reset();
}