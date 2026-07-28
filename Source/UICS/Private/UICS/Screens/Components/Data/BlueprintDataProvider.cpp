// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Data/BlueprintDataProvider.h"
#include "UICS/Screens/Components/Data/DataScreenComponent.h"
#include "UICS/Screens/Components/IUICSAccessor.h"

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

void UBlueprintDataProvider::NativeBeginRetrieveEntries()
{
	Super::NativeBeginRetrieveEntries();
	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_BeginRetrieveEntries))
	{
		struct FBlueprintDataProviderFunc {
			UUIDataProvider* DataProvider;
			UDataScreenComponent* ParentDataComponent;
			
		} Args = { this, GetParent()};

		ProcessFuncFromResolveMember(Func, &Args);
	}

}

void UBlueprintDataProvider::NativeRetrieveEntries(UDataScreenComponent* ComponentParent, TArray<UObject*>& RetrievedEntries)
{
	Super::NativeRetrieveEntries(ComponentParent, RetrievedEntries);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_RetrieveEntries))
	{
		TArray<UObject*> Temp;
		struct FBlueprintDataProviderFunc {
			UUIDataProvider* DataProvider;
			UDataScreenComponent* ParentDataComponent;
			TArray<UObject*> RetrievedEntries;

		} Args = { this, GetParent(), Temp};

		ProcessFuncFromResolveMember(Func, &Args);

		if(UDataScreenComponent* Parent = GetParent())
		{
			for (UObject* DataObj : Args.RetrievedEntries)
			{
				Parent->AddEntry(DataObj);
			}
		}
	}
}


void UBlueprintDataProvider::NativeEndRetrieveEntries()
{
	Super::NativeEndRetrieveEntries();
	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_EmdRetrieveEntries))
	{
		struct {
			UUIDataProvider* DataProvider;
			UDataScreenComponent* DataComponentParent;
		} Args = { this, GetParent()};

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

