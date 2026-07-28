// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/ScreenComponentWorldContext.h"
#include "UICS/Screens/Components/IUICSAccessor.h"
#include "UICS/Screens/UICSScreen.h"

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