// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UICS/Screens/Components/Data/UIDataProvider.h"
#include "PlayerStateDataProvider.generated.h"

// will retrieve all player states in the game
UCLASS()
class UICS_API UPlayerStateDataProvider : public UUIDataProvider
{
	GENERATED_BODY()

	virtual void NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) override;
};
