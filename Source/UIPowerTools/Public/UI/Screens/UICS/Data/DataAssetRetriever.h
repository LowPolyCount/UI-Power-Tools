// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Screens/UICS/Data/DataScreenComponentProvider.h"
#include "DataAssetRetriever.generated.h"

class UDataScreenComponent;

// defines an Array of Data Assets we can use for this example project
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class UIPOWERTOOLS_API UEntryDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Array of Objects to provide as Data, can be edited directly from a DataComponent in the editor
	// @todo: Should this change to use TSoftObjPtr?
	UPROPERTY(Instanced, EditAnywhere, Category = DataScreenComponent)
	TArray<UObject*> Entries;
};

// Allows you to select an existing UEntryDataAsset as your source of Data
UCLASS(EditInlineNew)
class UIPOWERTOOLS_API UDataAssetRetriever : public UDataScreenComponentProvider
{
	GENERATED_BODY()
public:
	UDataAssetRetriever(const FObjectInitializer& ObjectInitializer);
	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) override;

protected:
	// You can Provide an existing Data Asset, or edit the data entries you want directly in the editor
	UPROPERTY(Instanced, EditAnywhere, Category = DataScreenComponent, Meta=(ShowInnerProperties))
	TObjectPtr<UEntryDataAsset> DataAssetToUse;
};
