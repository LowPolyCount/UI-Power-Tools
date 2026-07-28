// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UICS/Screens/Components/Action/ActionScreenComponentProvider.h"
#include "BlueprintActionprovider.generated.h"

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct FBindableBlueprintActionProviderBindings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UICS.BlueprintActionProvider.Binding_CanExecuteAction", DefaultBindingName = "CanExecuteAction"))
	FMemberReference CanExecuteAction;

	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UICS.BlueprintActionProvider.Binding_ExecuteAction", DefaultBindingName = "ExecuteAction"))
	FMemberReference ExecuteAction;

};


/**
 * A Action Provider where the functions can be created in Blueprint
 */
UCLASS()
class UICS_API UBlueprintActionProvider : public UActionScreenComponentProvider
{
	GENERATED_BODY()
public:

	virtual bool NativeCanExecuteAction(UObject* Entry) override;
	virtual bool NativeExecuteAction(UObject* Entry) override;

protected:

	UFunction* ResolveMemberReference(const FMemberReference& Ref);
	void ProcessFuncFromResolveMember(UFunction* Func, void* Args);

	UPROPERTY(EditAnywhere, Category = ActionScreenComponent, Meta=(FullyExpand = true, DisplayName="Events"))
	FBindableBlueprintActionProviderBindings BindableEvents;


#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	bool Binding_CanExecuteAction(UActionScreenComponentProvider* CallingActionProvider, UActionScreenComponent* ParentActionComponent, UObject* Entry, const UObject* HiddenWorldContextObject) {return false;}

	UFUNCTION(BlueprintInternalUseOnly)
	bool Binding_ExecuteAction(UActionScreenComponentProvider* CallingActionProvider, UActionScreenComponent* ParentActionComponent, UObject* Entry, const UObject* HiddenWorldContextObject) {return false;}
#endif
};
