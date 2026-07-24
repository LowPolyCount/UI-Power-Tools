// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/UIDataProvider.h"
#include "UI/Screens/UICS/DataScreenComponent.h"

void UUIDataProvider::Initialize(UDataScreenComponent* InOwningParent)
{
	ParentComponent = MakeWeakObjectPtr(InOwningParent);
}

void UUIDataProvider::NativeConstruct()
{
	BP_Construct();
}

void UUIDataProvider::NativeDestruct()
{
	BP_Destruct();
}


UDataScreenComponent* UUIDataProvider::GetParent() const
{
	TStrongObjectPtr<UDataScreenComponent> FoundObjectPinned = ParentComponent.Pin();
	return FoundObjectPinned ? FoundObjectPinned.Get() : nullptr;
}


void UUIDataProvider::NativeBeginRetrieveEntries()
{
	BP_BeginRetrieveEntries();
}

// retrieve entries and place them in the given TArray
void UUIDataProvider::NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UUIDataProvider, BP_RetrieveEntries)))
	{
		BP_RetrieveEntries(Component, RetrievedEntries);
	}
}

void UUIDataProvider::NativeEndRetrieveEntries()
{
	BP_EndRetrieveEntries();
}