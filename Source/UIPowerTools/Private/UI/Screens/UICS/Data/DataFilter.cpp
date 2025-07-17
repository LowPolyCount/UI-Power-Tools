// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/DataFilter.h"

TArray<UObject*> UDataFilter::FilterEntries(const TArray<UObject*>& RetrievedEntries)
{
	TArray<UObject*> RetVal;
	for (UObject* Entry : RetrievedEntries)
	{
		if (ApplyFilter(Entry))
		{
			RetVal.Emplace(Entry);
		}
	}
	return RetVal;
}