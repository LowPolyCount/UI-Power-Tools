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

	virtual void TransformEntries(TArray<UObject*>& InRetrievedEntries);

	UFUNCTION(BlueprintNativeEvent, Category = DataScreenComponent)
	void Setup();
	UFUNCTION(BlueprintNativeEvent, Category = DataScreenComponent)
	void Teardown();
protected:
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Transform Entries"))
	void BP_TransformEntries();

	virtual void Setup_Implementation() {}
	virtual void Teardown_Implementation() {}

	UPROPERTY(BlueprintReadWrite)
	TArray<UObject*> RetrievedEntries;
};
