// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "EntryScreenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEntryDelegate, UEntryScreenComponent*, Component, UObject*, OldEntry, UObject*, NewEntry);

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct UIPOWERTOOLS_API FBindableEntryEvents
{
	GENERATED_BODY()

	// we called ExecuteAction() - what was the result?
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.EntryScreenComponent.Binding_OnEntryChange", DefaultBindingName="EntryChange", DisplayName = "OnEntryChange"))
	FMemberReference  Bind_OnEntryChange;
};

// contains an entry (one piece of data) that can be held and accessed by other components
UCLASS(BlueprintType)
class UIPOWERTOOLS_API UEntryScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:
	// called when the entry we hold changes
	UPROPERTY(BlueprintAssignable, Category=EntryScreenComponent)
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

	UPROPERTY(EditAnywhere, Category = EntryScreenComponent, Meta=(DisplayName="Events"));
	FBindableEntryEvents BindableEvents;

	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_OnEntryChange(UEntryScreenComponent* Component, UObject* OldEntry, UObject* NewEntry) {}
#endif // WITH_EDITOR
};