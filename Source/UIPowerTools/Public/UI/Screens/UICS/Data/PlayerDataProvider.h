// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/Data/DataScreenComponentProvider.h"
#include "PlayerDataProvider.generated.h"

// will retrieve all players in the game
UCLASS()
class UIPOWERTOOLS_API UPlayerDataProvider : public UDataScreenComponentProvider
{
	GENERATED_BODY()

	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) override;
};
