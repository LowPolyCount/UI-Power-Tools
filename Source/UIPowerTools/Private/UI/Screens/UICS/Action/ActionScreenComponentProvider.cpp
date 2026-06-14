// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"


void UActionScreenComponentProvider::Initialize(UActionScreenComponent* InOwner)
{
	Owner = InOwner;
}

bool UActionScreenComponentProvider::CanExecuteAction(UObject* Entry)
{
	FGameplayTag LastTag = UICS_ACTION_Default;
	return CanExecuteActionInternal(Entry);
}

bool UActionScreenComponentProvider::ExecuteAction(UObject* Entry)
{ 
	FGameplayTag LastTag = UICS_ACTION_Default;
	return ExecuteActionInternal(Entry);
}

bool UActionScreenComponentProvider::CanExecuteActionInternal_Implementation(UObject* Entry)
{
	return false;
}

bool UActionScreenComponentProvider::ExecuteActionInternal_Implementation(UObject* Entry)
{
	return false;
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