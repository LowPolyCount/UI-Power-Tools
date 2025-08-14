// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/DataScreenComponentProvider.h"

void UDataScreenComponentProvider::Setup()
{
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_Setup")))
	{
		BP_Setup();
	}
}

// retrieve entries and place them in the given TArray
void UDataScreenComponentProvider::RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_RetrieveEntries")))
	{
		BP_RetrieveEntries(Component, RetrievedEntries);
	}
}

void UDataScreenComponentProvider::Teardown()
{ 
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_Teardown")))
	{
		BP_Teardown();
	}
}
