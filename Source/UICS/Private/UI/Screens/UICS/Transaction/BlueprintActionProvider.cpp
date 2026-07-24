// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/BlueprintActionprovider.h"

#include "UI/Screens/UICS/IUICSAccessor.h"

UFunction* UBlueprintActionProvider::ResolveMemberReference(const FMemberReference& Ref)
{
	UFunction* RetVal = nullptr;
	if (UObjectBaseUtility* Screen = GetImplementingOuterObject(UUICSScreenAccessor::StaticClass()))
	{
		RetVal = Ref.ResolveMember<UFunction>(Screen->GetClass());
	}

	return RetVal;
}

void UBlueprintActionProvider::ProcessFuncFromResolveMember(UFunction* Func, void* Args)
{
	if (UObject* Screen = Cast<UObject>(GetImplementingOuterObject(UUICSScreenAccessor::StaticClass())))
	{
		Screen->ProcessEvent(Func, Args);
	}
}

bool UBlueprintActionProvider::NativeCanExecuteAction(UObject* Entry)
{
	bool bRetVal = Super::NativeCanExecuteAction(Entry);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.CanExecuteAction))
	{
		struct FParams {
			UActionScreenComponentProvider* CallingActionProvider;
			UActionScreenComponent* ParentActionComponent;
			UObject* Entry;
			} Args = { this, GetParent(), Entry };

		ProcessFuncFromResolveMember(Func, &Args);

		if (const FBoolProperty* AsBool = CastField<FBoolProperty>(Func->GetReturnProperty()))
		{
			bRetVal = AsBool->GetPropertyValue(Func);
		}
	}

	return bRetVal;
}

bool UBlueprintActionProvider::NativeExecuteAction(UObject* Entry)
{
	bool bRetVal = Super::NativeExecuteAction(Entry);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.ExecuteAction))
	{
		struct FParams {
			UActionScreenComponentProvider* CallingActionProvider;
			UActionScreenComponent* ParentActionComponent;
			UObject* Entry;
		} Args = { this, GetParent(), Entry };

		ProcessFuncFromResolveMember(Func, &Args);

		if (const FBoolProperty* AsBool = CastField<FBoolProperty>(Func->GetReturnProperty()))
		{
			bRetVal = AsBool->GetPropertyValue(Func);
		}
	}

	return bRetVal;
}