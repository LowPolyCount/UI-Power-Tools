// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Data/BlueprintDataProvider.h"
#include "UI/Screens/UICS/IUICSAccessor.h"

UFunction* UBlueprintDataProvider::ResolveMemberReference(const FMemberReference& Ref)
{
	UFunction* RetVal = nullptr;
	if (UObjectBaseUtility* Screen = GetImplementingOuterObject(UUICSScreenAccessor::StaticClass()))
	{
		RetVal = Ref.ResolveMember<UFunction>(Screen->GetClass());
	}

	return RetVal;
}

void UBlueprintDataProvider::ProcessFuncFromResolveMember(UFunction* Func, void* Args)
{
	if (UObject* Screen = Cast<UObject>(GetImplementingOuterObject(UUICSScreenAccessor::StaticClass())))
	{
		Screen->ProcessEvent(Func, Args);
	}
}

// run before Retrieve Entries to handle any setup needed
void UBlueprintDataProvider::NativeBeginRetrieveEntries()
{
	Super::NativeBeginRetrieveEntries();
	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_Setup))
	{
		struct {
			
		} Args = {  };

		ProcessFuncFromResolveMember(Func, &Args);
	}

}

void UBlueprintDataProvider::NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	Super::NativeRetrieveEntries(Component, RetrievedEntries);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_RetrieveEntries))
	{
		struct {
			UDataScreenComponent* Component;
			TArray<UObject*>& RetrievedEntries;
		} Args = { Component,  RetrievedEntries };

		ProcessFuncFromResolveMember(Func, &Args);
	}

}


void UBlueprintDataProvider::NativeEndRetrieveEntries()
{
	Super::NativeEndRetrieveEntries();
	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_TearDown))
	{
		struct {

		} Args = {  };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

