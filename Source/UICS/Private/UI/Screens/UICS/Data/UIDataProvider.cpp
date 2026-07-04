// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/UIDataProvider.h"


void UUIDataProvider::NativeConstruct()
{
	BP_Construct();
}

void UUIDataProvider::NativeDestruct()
{
	BP_Destruct();
}


void UUIDataProvider::NativeBeginRetrieveEntries()
{
	BP_BeginRetrieveEntries();
}

// retrieve entries and place them in the given TArray
void UUIDataProvider::NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUIDataProvider, BP_RetrieveEntries)))
	{
		BP_RetrieveEntries(Component, RetrievedEntries);
	}
}

void UUIDataProvider::NativeEndRetrieveEntries()
{
	BP_EndRetrieveEntries();
}