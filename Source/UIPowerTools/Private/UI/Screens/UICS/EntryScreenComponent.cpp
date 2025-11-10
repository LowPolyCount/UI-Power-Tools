// Copyright (c) Joel Gonzales


#include "UI/Screens/UICS/EntryScreenComponent.h"

void UEntryScreenComponent::SetEntry(UObject* Entry) 
{
	const bool bBroadcast = (bBroadcastOnAnyEntryChange) ? true : HeldEntry != Entry;
	UObject* OldEntry = HeldEntry;

	HeldEntry = Entry;

	if (bBroadcast)
	{
		OnEntryChange.Broadcast(this, OldEntry, HeldEntry);
	}
}