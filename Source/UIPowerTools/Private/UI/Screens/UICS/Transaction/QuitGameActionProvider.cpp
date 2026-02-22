// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/QuitGameActionProvider.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"

FGameplayTag UQuitGameActionProvider::ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry)
{
	// we assume the data sent in may be an APlayerController. You can override this assumption in your game. 
	APlayerController* OptionalPlayerController = Cast<APlayerController>(Entry);

	UKismetSystemLibrary::QuitGame(this, OptionalPlayerController, QuitPreference, bIgnorePlatformRestrictions);

	// always assume success
	return UICS_Action_Success;
}
