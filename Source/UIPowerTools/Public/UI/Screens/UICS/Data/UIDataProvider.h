// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "UIDataProvider.generated.h"

class UDataScreenComponent;

// Used by the DataScreenComponent to retrieve data and put it in the given TArray
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class UIPOWERTOOLS_API UUIDataProvider : public UScreenComponentWorldContext
{
	GENERATED_BODY()
public:
	// run before Retrieve Entries to handle any setup needed
	virtual void Setup();

	// retrieve entries and add them in the given TArray. 
	// @param Component the component we belong to
	// @param RetrievedEntries The Data Entries to be returned.  Assume the array is not empty
	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries);

	// run after RetrieveEntries to handle any teardown needed
	virtual void Teardown();

protected:
	// run before Retrieve Entries to handle any setup needed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Setup"))
	void BP_Setup();

	// retrieve entries and add them in the given TArray. Assume RetrievedEntries is not empty
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="RetrieveEntries"))
	void BP_RetrieveEntries(UDataScreenComponent* Component, UPARAM(ref) TArray<UObject*>& RetrievedEntries);

	// run after RetrieveEntries to handle any teardown needed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Teardown"))
	void BP_Teardown();
};
