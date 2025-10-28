// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/UIDataProvider.h"

void UUIDataProvider::Setup()
{
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUIDataProvider, BP_Setup)))
	{
		BP_Setup();
	}
}

// retrieve entries and place them in the given TArray
void UUIDataProvider::RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUIDataProvider, BP_RetrieveEntries)))
	{
		BP_RetrieveEntries(Component, RetrievedEntries);
	}
}

void UUIDataProvider::Teardown()
{ 
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUIDataProvider, BP_Teardown)))
	{
		BP_Teardown();
	}
}
