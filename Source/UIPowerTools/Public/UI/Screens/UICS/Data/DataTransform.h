// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "DataTransform.generated.h"

// takes an array of data and can transform it by sorting it, rearranging, condensing, etc.
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UDataTransform : public UScreenComponentWorldContext
{
	GENERATED_BODY()
public:

	// called before TransformEntries happens to allow you to setup anything like variables required for filtering
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Begin Transform Entries"))
	void BP_BeginTransformEntries();
	virtual void NativeBeginTransformEntries() {BP_BeginTransformEntries(); }

	UFUNCTION(BlueprintImplementableEvent, Category = DataScreenComponent, Meta=(DisplayName="Transform Entries"))
	void BP_TransformEntries();
	virtual void NativeTransformEntries(TArray<UObject*>& InRetrievedEntries);

	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="End Transform Entries"))
	void BP_EndTransformEntries();
	virtual void NativeEndTransformEntries() { BP_EndTransformEntries(); }

protected:
	UPROPERTY(BlueprintReadWrite, Category = DataScreenComponent)
	TArray<UObject*> RetrievedEntries;
};
