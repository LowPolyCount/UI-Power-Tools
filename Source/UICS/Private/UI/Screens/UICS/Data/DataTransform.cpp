// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/DataTransform.h"

void UDataTransform::NativeTransformEntries(TArray<UObject*>& InRetrievedEntries)
{	
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UDataTransform, BP_TransformEntries)))
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
