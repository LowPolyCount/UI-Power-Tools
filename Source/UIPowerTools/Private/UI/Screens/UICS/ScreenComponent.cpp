// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screens/UICS/ScreenComponent.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
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

UFunction* UScreenComponent::ResolveMemberReference(const FMemberReference& Ref)
{
	UFunction* RetVal = nullptr;
	if (UObjectBaseUtility* Screen = GetImplementingOuterObject(UUICSScreenAccessor::StaticClass()))
	{
		RetVal = Ref.ResolveMember<UFunction>(Screen->GetClass());
	}
	
	return RetVal;
}

void UScreenComponent::ProcessFuncFromResolveMember(UFunction* Func, void* Args)
{
	if (UObject* Screen = Cast<UObject>(GetImplementingOuterObject(UUICSScreenAccessor::StaticClass())))
	{
		Screen->ProcessEvent(Func, Args);
	}
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