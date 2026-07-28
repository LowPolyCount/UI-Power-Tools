// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Action/QuitGameActionProvider.h"
#include "UICS/Screens/Components/Action/ActionScreenComponent.h"

bool UQuitGameActionProvider::ExecuteActionInternal_Implementation(UObject* Entry)
{
	// we assume the data sent in may be an APlayerController. You can override this assumption in your game. 
	APlayerController* OptionalPlayerController = Cast<APlayerController>(Entry);

	UKismetSystemLibrary::QuitGame(this, OptionalPlayerController, QuitPreference, bIgnorePlatformRestrictions);

	SetActionResult(UICS_Action_Success);
	// always assume success
	return true;
}
