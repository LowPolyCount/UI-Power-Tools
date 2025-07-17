// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/PlayerDataProvider.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

void UPlayerDataProvider::RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	const int32 NumPlayers = UGameplayStatics::GetNumPlayerStates(this);
	
	for (int32 i = 0; i < NumPlayers; ++i)
	{
		if (APlayerState* State = UGameplayStatics::GetPlayerState(this, i))
		{
			RetrievedEntries.Emplace(State);
		}
	}
}