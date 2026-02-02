// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/QuitGameActionProvider.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"

ETransactionResult UQuitGameActionProvider::ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry)
{
	// we assume the data sent in may be an APlayerController. You can override this assumption in your game. 
	APlayerController* OptionalPlayerController = Cast<APlayerController>(Entry);

	UKismetSystemLibrary::QuitGame(this, OptionalPlayerController, QuitPreference, bIgnorePlatformRestrictions);
	return ETransactionResult::Success;
}
