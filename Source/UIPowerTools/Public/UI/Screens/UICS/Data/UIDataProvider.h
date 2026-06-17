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
	// called when NativeConstruct happens on the Screen/Widget the component lives on
	// @note: Useful when you need to setup listeners for data that may change. 
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Construct"))
	void BP_Construct();
	virtual void NativeConstruct();

	// called when NativeDestruct happens on the Screen/Widget the component lives on
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Destruct"))
	void BP_Destruct();
	virtual void NativeDestruct();

	// retrieve entries and add them in the given TArray. 
	// @param Component the component we belong to
	// @param RetrievedEntries The Data Entries to be returned.  Assume the array is not empty
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Begin Retrieve Entries"))
	void BP_BeginRetrieveEntries();
	virtual void NativeBeginRetrieveEntries();

	// retrieve entries and add them in the given TArray. Assume RetrievedEntries is not empty
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="RetrieveEntries"))
	void BP_RetrieveEntries(UDataScreenComponent* Component, UPARAM(ref) TArray<UObject*>& RetrievedEntries);
	virtual void NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries);

	// run after RetrieveEntries to handle any teardown needed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="End Retrieve Entries"))
	void BP_EndRetrieveEntries();
	virtual void NativeEndRetrieveEntries();
};
