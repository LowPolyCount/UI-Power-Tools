// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Action/ActionScreenComponentProvider.h"
#include "UICS/Screens/Components/Action/ActionScreenComponent.h"


void UActionScreenComponentProvider::NativeInitialize(UActionScreenComponent* InOwner)
{
	ParentComponent = MakeWeakObjectPtr(InOwner);
}

UActionScreenComponent* UActionScreenComponentProvider::GetParent() const
{
	TStrongObjectPtr<UActionScreenComponent> FoundObjectPinned = ParentComponent.Pin();
	return FoundObjectPinned ? FoundObjectPinned.Get() : nullptr;
}


bool UActionScreenComponentProvider::NativeCanExecuteAction(UObject* Entry)
{
	FGameplayTag LastTag = UICS_ACTION_Default;

	bool bRetVal = false;
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UActionScreenComponentProvider, BP_CanExecuteAction)))
	{
		bRetVal = BP_CanExecuteAction(Entry);
	}

	else
	{
		bRetVal = CanExecuteActionInternal(Entry);
	}
	return bRetVal;
}

bool UActionScreenComponentProvider::NativeExecuteAction(UObject* Entry)
{ 
	FGameplayTag LastTag = UICS_ACTION_Default;

	bool bRetVal = false;
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UActionScreenComponentProvider, BP_ExecuteAction)))
	{
		bRetVal = BP_ExecuteAction(Entry);
	}
	else
	{
		bRetVal = ExecuteActionInternal(Entry);
	}
	return bRetVal;
}

bool UActionScreenComponentProvider::HasTextAssociatedWithLastActionResultTag() const
{
	return ActionResultTagToText.Contains(GetLastActionResult());
}

FText UActionScreenComponentProvider::GetTextAssociatedWithLastActionResultTag() const
{
	FText RetVal = FText::GetEmpty();
	if(HasTextAssociatedWithLastActionResultTag())
	{
		RetVal = ActionResultTagToText[GetLastActionResult()];
	}

	return RetVal;
}