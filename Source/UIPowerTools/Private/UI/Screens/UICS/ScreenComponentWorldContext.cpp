// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "UI/Screens/Screen.h"

UWorld* UScreenComponentWorldContext::GetWorld() const
{
	UWorld* RetVal = nullptr;
	if (const UScreen* OwningScreen = GetTypedOuter<UScreen>())
	{
		RetVal = OwningScreen->GetWorld();
	}
	// @todo: we should always be attached to a screen. log if that doesn't happen

	return RetVal;
}