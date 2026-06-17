// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "DataFilter.generated.h"

// take an array of data and will filter it with given criteria
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UDataFilter : public UScreenComponentWorldContext
{
	GENERATED_BODY()
public:
	// called before FilterEntries happens to allow you to setup anything like variables required for filtering
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Begin Filter Entries"))
	void BP_BeginFilterEntries();
	virtual void NativeBeginFilterEntries();

	// run NativeApplyFilter on all RetirevedEntries 
	TArray<UObject*> FilterEntries(const TArray<UObject*>& RetrievedEntries);

	// called after FilterEntries to do any cleanup required.
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="End Filter Entries"))
	void BP_EndFilterEntries();
	virtual void NativeEndFilterEntries();
protected:

	// Filter the given Entry. Return false if given entry is removed
	UFUNCTION(BlueprintImplementableEvent, Category = DataScreenComponent, Meta = (DisplayName = "Apply Filter"))
	bool BP_ApplyFilter(const UObject* Entry);
	virtual bool NativeApplyFilter(const UObject* Entry);

	bool bBlueprintApplyFilter = false;
};
