// Copyright 2025 Joel Gonzales


#include "UI/Utility/UIPTStatics.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "Components/Widget.h"
#include "UI/Screens/UICS/IUICSAccessor.h"

UScreenComponent* UUIPTStatics::GetScreenComponent_BP(const UObject* PathToScreen, TSubclassOf<UScreenComponent> Type)
{
	UScreenComponent* RetVal = nullptr;

	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetScreenComponent_BP(Type);
	}
	return RetVal;
}

TArray<UScreenComponent*> UUIPTStatics::GetAllScreenComponents_BP(const UObject* PathToScreen, TSubclassOf<UScreenComponent> Type)
{
	TArray<UScreenComponent*> RetVal;
	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetAllScreenComponents_BP(Type);
	}
	return RetVal;
}

UScreenComponent* UUIPTStatics::GetScreenComponentByName_BP(const UObject* PathToScreen, const FName Name, TSubclassOf<UScreenComponent> Type)
{
	UScreenComponent* RetVal = nullptr;
	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetScreenComponentByName_BP(Name, Type);
	}
	return RetVal;
}

UScreenComponent* UUIPTStatics::GetScreenComponentFromSelector_BP(const UObject* PathToScreen, const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type)
{
	UScreenComponent* RetVal = nullptr;
	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetScreenComponentFromSelector_BP(Selector, Type);
	}
	return RetVal;
}

TScriptInterface<const IUICSScreenAccessor> UUIPTStatics::GetScreenAccessor(const UObject* PathToScreen)
{
	TScriptInterface<const IUICSScreenAccessor> RetVal = nullptr;
	if (PathToScreen)
	{
		if (const IUICSScreenAccessor* AsScreen = Cast<IUICSScreenAccessor>(PathToScreen))
		{
			RetVal = PathToScreen;
		}
		else
		{
			UObjectBaseUtility* Outer = PathToScreen->GetImplementingOuterObject(UUICSScreenAccessor::StaticClass());
			RetVal = TScriptInterface<const IUICSScreenAccessor>(Cast<UObject>(Outer));
		}
	}

	// don't log or error if RetVal is null. Can happen in editor
	return RetVal;
}


UWidget* UUIPTStatics::GetDesiredFocusTargetFromViewComponents(const UObject* PathToScreen)
{
	UWidget* RetVal = nullptr;
	for (const UViewScreenComponent* ViewComponent : UUIPTStatics::GetAllScreenComponents<UViewScreenComponent>(PathToScreen))
	{
		if (IsValid(ViewComponent) && ViewComponent->IsDesiredFocusTarget())
		{
			RetVal = ViewComponent->GetDesiredFocusTarget();
			if (IsValid(RetVal))
			{
				break;
			}
		}
	}
	return RetVal;
}
