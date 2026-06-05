// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/QuitGameActionProvider.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"

bool UQuitGameActionProvider::ExecuteActionInternal_Implementation(UObject* Entry)
{
	// we assume the data sent in may be an APlayerController. You can override this assumption in your game. 
	APlayerController* OptionalPlayerController = Cast<APlayerController>(Entry);

	UKismetSystemLibrary::QuitGame(this, OptionalPlayerController, QuitPreference, bIgnorePlatformRestrictions);

	SetExecuteResultTag(UICS_Action_Success);
	// always assume success
	return true;
}
