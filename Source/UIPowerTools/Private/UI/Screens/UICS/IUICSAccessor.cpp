// Copyright (c) Joel Gonzales

#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UIPowerTools.h"
#include "UI/Screens/Screen.h"

UScreenComponent* IUICSAccessor::GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	if (UScreen* Screen = GetScreen())
	{
		RetVal = Screen->GetScreenComponent_BP(Type);
	}
	return RetVal;
}

TArray<UScreenComponent*> IUICSAccessor::GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const
{
	TArray<UScreenComponent*> RetVal;
	if (UScreen* Screen = GetScreen())
	{
		RetVal = Screen->GetAllScreenComponents_BP(Type);
	}
	return RetVal;
}

UScreenComponent* IUICSAccessor::GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	if (UScreen* Screen = GetScreen())
	{
		RetVal = Screen->GetScreenComponentFromSelector_BP(Selector, Type);
	}
	return RetVal;
}

UScreen* IUICSAccessor::GetScreen() const
{
	UScreen* RetVal = nullptr;
	if (const UObject* AsObject = Cast<UObject>(this))
	{
		RetVal = AsObject->GetTypedOuter<UScreen>();
		if (!RetVal)
		{
			UE_LOG(LogUICS, Warning, TEXT("Could not get owning Screen from %s - GetComponent() functions will not work"), *AsObject->GetFName().ToString());
		}
	}

	return RetVal;
}
