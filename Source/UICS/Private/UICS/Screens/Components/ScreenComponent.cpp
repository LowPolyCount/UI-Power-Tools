// Fill out your copyright notice in the Description page of Project Settings.


#include "UICS/Screens/Components/ScreenComponent.h"
#include "UICS/Screens/Components/IUICSAccessor.h"
#include "UObject/UObjectGlobals.h"

UScreenComponent::UScreenComponent(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
	//ComponentName = GetFName();
	////RemoveClassPrefix();
	ComponentName = MakeUniqueObjectName(this, Initializer.GetClass(), NAME_None, EUniqueObjectNameOptions::GloballyUnique);
}

void UScreenComponent::NativeInitialize() 
{
	Initialize();
}

void UScreenComponent::NativePreConstruct(bool bIsDesignTime)
{
	PreConstruct(bIsDesignTime);
}

void UScreenComponent::NativeConstruct() 
{
	Construct();
}

void UScreenComponent::NativeDestruct() 
{
	Destruct();
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

FString UScreenComponent::GetDisplayName() const
{
	return ComponentName.ToString() + TEXT(" ") + GetClass()->GetFName().ToString();
}

FString UScreenComponent::GetDisplayNameVerbose() const
{
	return ComponentName.ToString() + TEXT(" ") + GetClass()->GetFName().ToString();
}