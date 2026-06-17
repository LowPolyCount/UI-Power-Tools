// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/DataFilter.h"

void UDataFilter::NativeBeginFilterEntries()
{ 
	//@note: Could move IsFunctionImplementedInScript to initialize or something else only called once, but with how often this is called, it's not a big deal. 
	bBlueprintApplyFilter = GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UDataFilter, BP_ApplyFilter));

	BP_BeginFilterEntries(); 
}


void UDataFilter::NativeEndFilterEntries()
{
	BP_EndFilterEntries();
}

TArray<UObject*> UDataFilter::FilterEntries(const TArray<UObject*>& RetrievedEntries)
{
	TArray<UObject*> RetVal;
	for (UObject* Entry : RetrievedEntries)
	{
		if (NativeApplyFilter(Entry))
		{
			RetVal.Emplace(Entry);
		}
	}
	return RetVal;
}

bool UDataFilter::NativeApplyFilter(const UObject* Entry)
{
	bool bRetVal = true;
	if (bBlueprintApplyFilter)
	{
		bRetVal = BP_ApplyFilter(Entry);
	}
	return bRetVal;
}