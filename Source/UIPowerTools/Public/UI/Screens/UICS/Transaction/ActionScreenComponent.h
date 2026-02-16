// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "ActionScreenComponent.generated.h"

class UViewScreenComponent;
class UActionScreenComponentProvider;

// defines the results of a transaction
UENUM(BlueprintType)
enum class EActionResult : uint8
{
	CouldNotExecute,	// CanExecuteAction() was called and returned false 
	Success,
	Failure,
	Async			// The result will need to come back from the server through a callback. (Not yet supported)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValidTransactionResult, UActionScreenComponent*, Component, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTransactionResult, UActionScreenComponent*, Component, EActionResult, Result);

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct UIPOWERTOOLS_API FBindableActionEvents
{
	GENERATED_BODY()

	// we called ExecuteAction() - what was the result?
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ActionScreenComponent.Binding_TransactionResult", DefaultBindingName="ExecuteResult", DisplayName = "OnExecuteResult"))
	FMemberReference  Bind_OnExecuteResult;
};

// Transaction Component collects all the information required to make a change to the system and executes it using a transactor class
UCLASS()
class UIPOWERTOOLS_API UActionScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
protected:

	void Initialize() override;

public:
	// result of ExecuteTransaction
	UPROPERTY(BlueprintAssignable, Category = ActionScreenComponent)
	FTransactionResult OnExecuteResult;

	// Given the Entry Data, can we execute the current action?
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool CanExecuteAction(UObject* Entry = nullptr);

	// call CanExecuteAction() and if true, then call ExecuteAction()
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	EActionResult ExecuteActionIfAble(UObject* Entry = nullptr);

	// execute our action, without checking CanExecuteAction()
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	EActionResult ExecuteAction(UObject* Entry = nullptr);

	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetActionProvider(UActionScreenComponentProvider* InActionProvider) { ActionProvider = InActionProvider; }

	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	UActionScreenComponentProvider* GetActionProvider() const {return ActionProvider;}

	// listen for input events from the given View
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void ListenToViewAction(UViewScreenComponent* InView);

protected:
	UFUNCTION()
	void HandleOnInputAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);

	// The action provider implements the action that you want to take place
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = ActionScreenComponent, Meta=(Displayname="Action"))
	TObjectPtr<UActionScreenComponentProvider> ActionProvider = nullptr;

	// Will execute our action in response to the view screen component chosen
	UPROPERTY(EditAnywhere, Category = ActionScreenComponent)
	FViewComponentSelector ViewToListenTo;

	UPROPERTY(EditAnywhere, Category = ActionScreenComponent, Meta=(DisplayName="Events"));
	FBindableActionEvents BindableEvents;

	UPROPERTY()
	TObjectPtr<UViewScreenComponent> ViewListeningTo;

	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_IsTransactionValid(UActionScreenComponent* Component, bool bResult) {}
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_TransactionResult(UActionScreenComponent* Component, EActionResult Result) {}
#endif // WITH_EDITOR
	// END FMember References

public:
	// begin deprecated functions
	// 
	// result of IsValidTransaction
	UE_DEPRECATED(Any, "is deprecated and will be removed")
	UPROPERTY()
	FValidTransactionResult OnIsValidResult_DEPRECATED;

	// does the transaction have everything needed to be valid?
	UE_DEPRECATED(Any, "is deprecated. Use CanExecuteAction() instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta = (DeprecatedFunction, DeprecationMessage = "Use CanExecuteAction() instead"))
	bool IsValidTransaction(UObject* Entry);

	// set the transactor used
	UE_DEPRECATED(Any, "is deprecated. Use SetActionProvider instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta=(DeprecatedFunction, DeprecationMessage="Use SetActionProvider() instead"))
	void SetTransactor(UActionScreenComponentProvider* InTransaction) { ActionProvider = InTransaction; }

	// slots
	// set the data for a slot at the index
	UE_DEPRECATED(Any, "Slots are deprecated. Use properties on your Action Provider instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta=(DeprecatedFunction, DeprecationMessage = "Slots are deprecated. Use properties on your Action Provider instead"))
	void SetSlot(UObject* Entry, int32 Index);

	// remove the data for a slot at the index
	UE_DEPRECATED(Any, "Slots are deprecated. Use properties on your Action Provider instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta = (DeprecatedFunction, DeprecationMessage = "Slots are deprecated. Use properties on your Action Provider instead"))
	void RemoveSlot(int32 Index);

	// get the data for a slot at the index
	UE_DEPRECATED(Any, "Slots are deprecated. Use properties on your Action Provider instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta = (DeprecatedFunction, DeprecationMessage = "Slots are deprecated. Use properties on your Action Provider instead"))
	UObject* GetSlot(int32 Index) const;

	// does the slot at index have any data?
	UE_DEPRECATED(Any, "Slots are deprecated. Use properties on your Action Provider instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta = (DeprecatedFunction, DeprecationMessage = "Slots are deprecated. Use properties on your Action Provider instead"))
	bool IsSlotValid(int32 Index) const;

	// how many slots do we have?
	UE_DEPRECATED(Any, "Slots are deprecated. Use properties on your Action Provider instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, Meta = (DeprecatedFunction, DeprecationMessage = "Slots are deprecated. Use properties on your Action Provider instead"))
	int32 NumSlots() const;

	UE_DEPRECATED(Any, "Is Deprecated. Use HandleOnInputAction() Instead")
	UFUNCTION(Category = ActionScreenComponent, meta = (DeprecatedFunction, DeprecationMessage = "HandleOnAction is deprecated. Use HandleOnInputAction instead"))	
	void HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);

private:

	// slots are provided as a holding place for data. So you can put it in a slot, and then have the action provider retrieve it later. 
	UPROPERTY()
	TMap<int32, UObject*> Slots;


};

