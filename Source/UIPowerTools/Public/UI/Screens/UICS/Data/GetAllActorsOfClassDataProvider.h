// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/Data/DataScreenComponentProvider.h"
#include "GetAllActorsOfClassDataProvider.generated.h"

/**
 * Will retrieve all actors of a given type
 */
 UCLASS(Meta=(DisplayName="Get All Actors Of Class"))
class UIPOWERTOOLS_API UGetAllActorsOfClassDataProvider : public UDataScreenComponentProvider
{
	GENERATED_BODY()
public:
	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries);

protected:
	// the class that we will be retrieving all actors of
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor>	ActorClass;
};
