// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
//#include "Engine/MemberReference.h"
#include "UI/Screens/UICS/Data/UIDataProvider.h"
#include "BlueprintDataProvider.generated.h"

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct FBindableBlueprintDataProviderActions
{
	GENERATED_BODY()

	// an input action has occurred on a widget
	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UIPowerTools.UIDataProvider.BP_Setup", DefaultBindingName = "Construct", DisplayName="Construct"))
	FMemberReference Bind_Setup;

	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UIPowerTools.UIDataProvider.BP_RetrieveEntries", DefaultBindingName = "RetrieveEntries", DisplayName="RetrieveEntries"))
	FMemberReference Bind_RetrieveEntries;

	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UIPowerTools.UIDataProvider.BP_Teardown", DefaultBindingName = "Destroy", DisplayName="Destroy"))
	FMemberReference Bind_TearDown;
};


/**
 * a Data Provider that uses FMembers to allow a user to set blueprint functions on a screen as the overrides. 
 */
UCLASS()
class UIPOWERTOOLS_API UBlueprintDataProvider : public UUIDataProvider
{
	GENERATED_BODY()
public:

	virtual void NativeBeginRetrieveEntries() override;
	virtual void NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) override;
	virtual void NativeEndRetrieveEntries() override;

protected:
	UFunction* ResolveMemberReference(const FMemberReference& Ref);
	void ProcessFuncFromResolveMember(UFunction* Func, void* Args);

	// events that the user can bind to in editor
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent, Meta=(DisplayName="Events"));
	FBindableBlueprintDataProviderActions BindableEvents;
};
