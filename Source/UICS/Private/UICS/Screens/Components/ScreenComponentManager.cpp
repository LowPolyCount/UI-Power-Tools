// Copyright (c) Joel Gonzales


#include "UICS/Screens/Components/ScreenComponentManager.h"
#include "Templates/SubclassOf.h"
#include "UICS/Screens/Components/ScreenComponent.h"
#include "UICS/Screens/Tools/ComponentSelector.h"

#if WITH_EDITOR
UScreenComponent* UScreenComponentManager::GetComponentFromGUID(const FGuid& Selector) const
{
	UScreenComponent* RetVal = nullptr;
	for (UScreenComponent* Component : Components)
	{
		if (Component && Component->GetGuid() == Selector)
		{
			RetVal = Component;
			break;
		}
	}

	return RetVal;
}

void UScreenComponentManager::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	for (UScreenComponent* Component : Components)
	{
		if (Component)
		{
			Component->ValidateCompiledDefaults(CompileLog);
		}
	}
}
#endif

void UScreenComponentManager::Add(UScreenComponent* Component)
{
	Components.Emplace(Component);
}

UScreenComponent* UScreenComponentManager::GetComponent(TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component) && Component->IsA(Type))
		{
			RetVal = Component;
			break;
		}
	}

	return RetVal;
}

TArray<UScreenComponent*> UScreenComponentManager::GetAllComponents(TSubclassOf<UScreenComponent> Type) const
{
	TArray<UScreenComponent*> RetVal;
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component) && Component->IsA(Type))
		{
			RetVal.Emplace(Component);
		}
	}

	return RetVal;
}

UScreenComponent* UScreenComponentManager::GetComponentByName(const FName Name, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component) && Component->IsA(Type) && Component->GetComponentName() == Name)
		{
			RetVal = Component;
			break;
		}
	}

	return RetVal;
}

UScreenComponent* UScreenComponentManager::GetComponentFromSelector(const FComponentSelector& Selector) const
{
	UScreenComponent* RetVal = nullptr;
	const FGuid& SelectedGuid = Selector.GetComponentGuid();

	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component) && Component->GetGuid() == SelectedGuid)
		{
			RetVal = Component;
			break;
		}
	}

	return RetVal;
}

void UScreenComponentManager::NativeInitialize()
{
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component))
		{
			Component->NativeInitialize();
		}
	}
}

void UScreenComponentManager::NativePreConstruct(bool bIsDesignTime)
{
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component))
		{
			Component->NativePreConstruct(bIsDesignTime);
		}
	}
}

void UScreenComponentManager::NativeConstruct()
{
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component))
		{
			Component->NativeConstruct();
		}
	}
}

void UScreenComponentManager::NativeDestruct()
{
	for (UScreenComponent* Component : Components)
	{
		if (IsValid(Component))
		{
			Component->NativeDestruct();
		}
	}
}
