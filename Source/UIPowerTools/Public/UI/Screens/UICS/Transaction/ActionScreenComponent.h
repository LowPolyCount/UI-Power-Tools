// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "ActionScreenComponent.generated.h"

class UViewScreenComponent;
class UActionScreenComponentProvider;

// defines the results of a transaction
UENUM(BlueprintType, Meta=(ScriptName="TransactionResultEnum"))
enum class ETransactionResult : uint8
{
	Success,
	Failure,
	Async			// The result will need to come back from the server through a callback. (Not yet supported)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValidTransactionResult, UActionScreenComponent*, Component, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTransactionResult, UActionScreenComponent*, Component, ETransactionResult, Result);

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct FBindableActionEvents
{
	GENERATED_BODY()

	// Widgets have been created and populated
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ActionScreenComponent.Binding_IsTransactionValid", DefaultBindingName="IsValidResult", DisplayName = "OnIsValidResult"))
	FMemberReference  Bind_OnIsValidResult;

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
	// result of IsValidTransaction
	UPROPERTY(BlueprintAssignable, Category = ActionScreenComponent)
	FValidTransactionResult OnIsValidResult;

	// result of ExecuteTransaction
	UPROPERTY(BlueprintAssignable, Category = ActionScreenComponent)
	FTransactionResult OnExecuteResult;


	// does the transaction have everything needed to be valid?
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool IsValidTransaction(UObject* Entry);

	// execute the transaction, optionally calling IsValidTransaction()
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	ETransactionResult ExecuteAction(UObject* Entry, bool bCallIsValidTransactionFirst = true);

	// slots
	// set the data for a slot at the index
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetSlot(UObject* Entry, int32 Index);

	// remove the data for a slot at the index
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void RemoveSlot(int32 Index);

	// get the data for a slot at the index
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	UObject* GetSlot(int32 Index) const;

	// does the slot at index have any data?
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool IsSlotValid(int32 Index) const;

	// how many slots do we have?
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	int32 NumSlots() const;

	// listen for action events from the given View
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void ListenToViewAction(UViewScreenComponent* InView);

	// set the transactor used
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetTransactor(UActionScreenComponentProvider* InTransaction) { ActionProvider = InTransaction; }

protected:
	UFUNCTION()
	void HandleOnInputAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);
	// set the transactor that will be used
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = ActionScreenComponent, Meta=(Displayname="Action"))
	TObjectPtr<UActionScreenComponentProvider> ActionProvider = nullptr;

	UPROPERTY()
	TObjectPtr<UViewScreenComponent> ViewListeningTo;

	// view screen component that we will list to events from
	UPROPERTY(EditAnywhere, Category = ActionScreenComponent)
	FViewComponentSelector ViewToListenTo;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValidTransactionResult, UActionScreenComponent*, Component, bool, bResult);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTransactionResult, UActionScreenComponent*, Component, ETransactionResult, Result);

	UPROPERTY(EditAnywhere, Category = ActionScreenComponent, Meta=(DisplayName="Events"));
	FBindableActionEvents BindableEvents;

	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_IsTransactionValid(UActionScreenComponent* Component, bool bResult) {}
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_TransactionResult(UActionScreenComponent* Component, ETransactionResult Result) {}
#endif // WITH_EDITOR

	// slots are provided as a holding place for data. So you can put it in a slot, and then have the action provider retrieve it later. 
	UPROPERTY()
	TMap<int32, UObject*> Slots;

	// begin deprecated functions

	UE_DEPRECATED(Any, "Is Deprecated. Use HandleOnInputAction() Instead")
	UFUNCTION(Category = ActionScreenComponent, meta = (DeprecatedFunction, DeprecationMessage = "HandleOnAction is deprecated. Use HandleOnInputAction instead"))	
	void HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);
};

