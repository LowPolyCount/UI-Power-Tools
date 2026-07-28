// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Data/DataAssetRetriever.h"

UDataAssetRetriever::UDataAssetRetriever(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DataAssetToUse = ObjectInitializer.CreateDefaultSubobject<UEntryDataAsset>(this, MakeUniqueObjectName(this, UEntryDataAsset::StaticClass()));
}

void UDataAssetRetriever::NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	Super::NativeRetrieveEntries(Component, RetrievedEntries);
	if (DataAssetToUse)
	{
		for(UObject* AsObject : DataAssetToUse->Entries)
		{
			RetrievedEntries.Emplace(AsObject);
		}	
	}
}