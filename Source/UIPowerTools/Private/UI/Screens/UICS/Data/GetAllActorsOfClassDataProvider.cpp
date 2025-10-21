// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/GetAllActorsOfClassDataProvider.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void UGetAllActorsOfClassDataProvider::RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	Super::RetrieveEntries(Component, RetrievedEntries);
	TArray<AActor*> ActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, ActorArray);

	//@todo: investigate moveorcopy() or another alternative to speed this up
	RetrievedEntries.Reserve(RetrievedEntries.Num() + ActorArray.Num());
	for (AActor* Actor : ActorArray)
	{
		RetrievedEntries.Emplace(Actor);
	}
}