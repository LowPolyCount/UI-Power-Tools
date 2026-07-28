// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UICS/Screens/Components/Data/UIDataProvider.h"
#include "BlueprintDataProvider.generated.h"

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct FBindableBlueprintDataProviderBindings
{
	GENERATED_BODY()

	// an input action has occurred on a widget
	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UICS.BlueprintDataProvider.Binding_BeginRetrieveEntries", DefaultBindingName = "BeginRetrieveEntries", DisplayName="BeginRetrieveEntries"))
	FMemberReference Bind_BeginRetrieveEntries;

	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UICS.BlueprintDataProvider.Binding_RetrieveEntries", DefaultBindingName = "RetrieveEntries", DisplayName="RetrieveEntries"))
	FMemberReference Bind_RetrieveEntries;

	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UICS.BlueprintDataProvider.Binding_EndRetrieveEntries", DefaultBindingName = "EndRetrieveEntries", DisplayName="EndRetrieveEntries"))
	FMemberReference Bind_EmdRetrieveEntries;
};

/**
 * a Data Provider that uses FMembers to allow a user to set blueprint functions on a screen as the overrides. 
 */
UCLASS()
class UICS_API UBlueprintDataProvider : public UUIDataProvider
{
	GENERATED_BODY()
public:

	virtual void NativeBeginRetrieveEntries() override;
	virtual void NativeRetrieveEntries(UDataScreenComponent* ComponentOwner, TArray<UObject*>& RetrievedEntries) override;
	virtual void NativeEndRetrieveEntries() override;

protected:
	UFunction* ResolveMemberReference(const FMemberReference& Ref);
	void ProcessFuncFromResolveMember(UFunction* Func, void* Args);

	// events that the user can bind to in editor
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent, Meta=(FullyExpand = true, DisplayName="Events"))
	FBindableBlueprintDataProviderBindings BindableEvents;

	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_BeginRetrieveEntries(UUIDataProvider* CallingDataProvider, UDataScreenComponent* ParentDataComponent) {}

	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_RetrieveEntries(UUIDataProvider* CallingDataProvider, UDataScreenComponent* ParentDataComponent,  TArray<UObject*> RetrievedEntries) {}

	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_EndRetrieveEntries(UUIDataProvider* CallingDataProvider, UDataScreenComponent* ParentDataComponent) {}
#endif // WITH_EDITOR
	// END FMember References

};
