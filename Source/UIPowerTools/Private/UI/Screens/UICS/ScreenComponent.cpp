// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/UICS/ScreenComponent.h"

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
	return GetClass()->GetFName().ToString();
}

FString UScreenComponent::GetDisplayNameVerbose() const
{
	return GetClass()->GetFName().ToString();
}
#endif