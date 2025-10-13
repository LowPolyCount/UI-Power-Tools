// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/DataTransform.h"

void UDataTransform::TransformEntries(TArray<UObject*>& InRetrievedEntries)
{
	// we have to use the FName because we don't have access to the class function
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_TransformEntries")))
	{
		// Would love to send this into blueprint as a parameter, but BP is not handling this correctly, so as a backup
		// we copy the array. 
		// @todo: See if we can get around this somehow
		RetrievedEntries = InRetrievedEntries;
		BP_TransformEntries();
		InRetrievedEntries = RetrievedEntries;
		RetrievedEntries.Empty();
	}
}
