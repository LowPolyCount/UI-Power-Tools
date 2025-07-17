// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "EntryScreenComponent.generated.h"

// contains data representing an entry that can be accessed by other components
UCLASS(BlueprintType)
class UIPOWERTOOLS_API UEntryScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:
	// get the entry
	UFUNCTION(BlueprintCallable)
	UObject* Get() const { return HeldEntry; }

	// set the entry
	UFUNCTION(BlueprintCallable)
	void Set(UObject* Entry) { HeldEntry = Entry; }

protected:
	UPROPERTY()
	TObjectPtr<UObject> HeldEntry;
};