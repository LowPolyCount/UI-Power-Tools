// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "EntryScreenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEntryDelegate, UEntryScreenComponent*, Component, UObject*, OldEntry, UObject*, NewEntry);

// contains an entry (one piece of data) that can be held and accessed by other components
UCLASS(BlueprintType)
class UIPOWERTOOLS_API UEntryScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:

	FEntryDelegate OnEntryChange;
	// get the entry
	UFUNCTION(BlueprintCallable, Category = EntryScreenComponent)
	UObject* GetEntry() const { return HeldEntry; }

	// set the entry
	UFUNCTION(BlueprintCallable, Category = EntryScreenComponent)
	void SetEntry(UObject* Entry);

	UFUNCTION(BlueprintCallable, Category = EntryScreenComponent)
	bool GetBroadcastOnAnyEntryChange() const {return bBroadcastOnAnyEntryChange;}

	UFUNCTION(BlueprintCallable, Category = EntryScreenComponent)
	void SetBroadcastOnAnyEntryChange(bool bBroadcastSetting ) { bBroadcastOnAnyEntryChange = bBroadcastSetting;}

protected:
	// by default, will only broadcast on SetEntry when NewEntry != OldEntry. 
	// Turning this on will broadcast whenever SetEntry() is called
	UPROPERTY(EditAnywhere, Category = EntryScreenComponent)
	bool bBroadcastOnAnyEntryChange=false;

	UPROPERTY()
	TObjectPtr<UObject> HeldEntry;
};