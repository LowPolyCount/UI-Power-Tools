// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/UICS/ScreenComponent.h"
#include "UObject/UObjectGlobals.h"

UScreenComponent::UScreenComponent(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
	//ComponentName = GetFName();
	////RemoveClassPrefix();
	ComponentName = MakeUniqueObjectName(this, Initializer.GetClass(), NAME_None, EUniqueObjectNameOptions::GloballyUnique);
}

const FGuid& UScreenComponent::GetGuid() const
{
	if (!Guid.IsValid())
	{
		Guid = FGuid::NewGuid();
	}

	return Guid;
}

#ifdef WITH_EDITORONLY_DATA
FString UScreenComponent::GetDisplayName() const
{
	return ComponentName.ToString() + TEXT(" ") + GetClass()->GetFName().ToString();
}

FString UScreenComponent::GetDisplayNameVerbose() const
{
	return ComponentName.ToString() + TEXT(" ") + GetClass()->GetFName().ToString();
}
#endif