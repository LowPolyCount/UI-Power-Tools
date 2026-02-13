// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "UI/Screens/UICSScreen.h"

UWorld* UScreenComponentWorldContext::GetWorld() const
{
	UWorld* RetVal = nullptr;
	if (const UObjectBaseUtility* Outer = GetImplementingOuterObject(UUICSScreenAccessor::StaticClass()))
	{
		if (const UObject* AsObject = Cast<UObject>(Outer))
		{
			RetVal = AsObject->GetWorld();
		}
	}
	// sometimes we may not find the outer such as in preconstruct

	return RetVal;
}