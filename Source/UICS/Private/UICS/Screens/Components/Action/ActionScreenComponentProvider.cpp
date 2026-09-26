// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information


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
	LastActionResultTag = UICS_ACTION_Default;

	bLastActionResult = false;
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UActionScreenComponentProvider, BP_CanExecuteAction)))
	{
		bLastActionResult = BP_CanExecuteAction(Entry);
	}
	else
	{
		bLastActionResult = CanExecuteActionInternal(Entry);
	}

	return bLastActionResult;
}

bool UActionScreenComponentProvider::NativeExecuteAction(UObject* Entry)
{ 
	LastActionResultTag = UICS_ACTION_Default;

	bLastActionResult = false;
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UActionScreenComponentProvider, BP_ExecuteAction)))
	{
		bLastActionResult = BP_ExecuteAction(Entry);
	}
	else
	{
		bLastActionResult = ExecuteActionInternal(Entry);
	}
	return bLastActionResult;
}

FActionResultData UActionScreenComponentProvider::GetLastActionResult() const
{
	return FActionResultData(bLastActionResult, LastActionResultTag);
}

bool UActionScreenComponentProvider::HasTextAssociatedWithLastActionResultTag() const
{
	return ActionResultTagToText.Contains(LastActionResultTag);
}

FText UActionScreenComponentProvider::GetTextAssociatedWithLastActionResultTag() const
{
	FText RetVal = FText::GetEmpty();
	if(HasTextAssociatedWithLastActionResultTag())
	{
		RetVal = ActionResultTagToText[LastActionResultTag];
	}

	return RetVal;
}