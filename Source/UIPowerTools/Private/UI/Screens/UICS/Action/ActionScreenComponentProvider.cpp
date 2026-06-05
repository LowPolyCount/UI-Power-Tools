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

//@note: Disabling until after 1.0 release
/*FText UActionScreenComponentProvider::GetTextAssociatedWithTag(const FGameplayTag& Tag) const
{
	FText RetVal;
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetTextAssociatedWithTag")))
	{
		RetVal = BP_GetTextAssociatedWithTag(Tag);
	}
	else if(ResultTagToText.Contains(Tag))
	{
		RetVal = ResultTagToText[Tag];
	}

	return RetVal;
}*/