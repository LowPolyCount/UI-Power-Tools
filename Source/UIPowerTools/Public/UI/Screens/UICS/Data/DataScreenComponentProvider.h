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
	// run before Retrieve Entries to handle any setup needed
	virtual void Setup();

	// retrieve entries and place them in the given TArray
	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries);

	// run after RetrieveEntries to handle any teardown needed
	virtual void Teardown();

protected:
	// run before Retrieve Entries to handle any setup needed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Setup"))
	void BP_Setup();

	// retrieve entries and place them in the given TArray
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="RetrieveEntries"))
	void BP_RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries);

	// run after RetrieveEntries to handle any teardown needed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Teardown"))
	void BP_Teardown();
};
