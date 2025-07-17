// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "DataScreenComponentProvider.generated.h"

class UDataScreenComponent;

// Used by the DataScreenComponent to retrieve data and put it in the given TArray
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class UIPOWERTOOLS_API UDataScreenComponentProvider : public UObject
{
	GENERATED_BODY()
public:
	virtual void Setup() {}

	// retrieve entries and place them in 
	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) PURE_VIRTUAL(UDataScreenComponentProvider::RetrieveEntries);
	virtual void Teardown() {}
};
