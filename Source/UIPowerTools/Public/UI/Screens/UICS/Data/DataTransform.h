// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "DataTransform.generated.h"

// takes an array of data and can transform it by rearranging, condensing, etc.
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UDataTransform : public UScreenComponentWorldContext
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void TransformEntries(TArray<UObject*>& RetrievedEntries);
	UFUNCTION(BlueprintNativeEvent, Category = DataScreenComponent)
	void Setup();
	UFUNCTION(BlueprintNativeEvent, Category = DataScreenComponent)
	void Teardown();
protected:
	virtual void TransformEntries_Implementation(TArray<UObject*>& RetrievedEntries) {}
	virtual void Setup_Implementation() {}
	virtual void Teardown_Implementation() {}
};
