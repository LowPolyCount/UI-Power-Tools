// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/Data/UIDataProvider.h"
#include "PlayerStateDataProvider.generated.h"

// will retrieve all player states in the game
UCLASS()
class UIPOWERTOOLS_API UPlayerStateDataProvider : public UUIDataProvider
{
	GENERATED_BODY()

	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) override;
};
