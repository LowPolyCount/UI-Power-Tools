// Copyright 2025 Joel Gonzales

#pragma once

#include "DataFilter.generated.h"

// take an array of data and will filter it with given criteria
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UDataFilter : public UObject
{
	GENERATED_BODY()
public:
	virtual void Setup() {}
	virtual TArray<UObject*> FilterEntries(const TArray<UObject*>& RetrievedEntries);
	virtual void Teardown() {}
protected:
	// Filter the given Entry. Return false if given entry is removed
	UFUNCTION(BlueprintNativeEvent, Category = DataScreenComponent)
	bool ApplyFilter(const UObject* Entry);

	virtual bool ApplyFilter_Implementation(const UObject* Entry) { return true; }
};
