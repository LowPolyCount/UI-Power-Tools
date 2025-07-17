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

// Transaction Component collects all the information required to make a change to the system and executes it using a transactor class
UCLASS()
class UIPOWERTOOLS_API UActionScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
protected:

	void Initialize() override;

public:
	// result of IsValidTransaction
	UPROPERTY(BlueprintAssignable)
	FValidTransactionResult OnIsValidResult;

	// result of ExecuteTransaction
	UPROPERTY(BlueprintAssignable)
	FTransactionResult OnExecuteResult;


	// does the transaction have everything needed to be valid?
	UFUNCTION(BlueprintCallable)
	bool IsValidTransaction(UObject* Entry);

	// execute the transaction, optionally calling IsValidTransaction()
	UFUNCTION(BlueprintCallable)
	ETransactionResult ExecuteAction(UObject* Entry, bool bCallIsValidTransactionFirst = true);

	// slots
	// set the data for a slot at the index
	UFUNCTION(BlueprintCallable)
	void SetSlot(UObject* Entry, int32 Index);

	// remove the data for a slot at the index
	UFUNCTION(BlueprintCallable)
	void RemoveSlot(int32 Index);

	// get the data for a slot at the index
	UFUNCTION(BlueprintCallable)
	UObject* GetSlot(int32 Index) const;

	// does the slot at index have any data?
	UFUNCTION(BlueprintCallable)
	bool IsSlotValid(int32 Index) const;

	// how many slots do we have?
	UFUNCTION(BlueprintCallable)
	int32 NumSlots() const;

	// listen for action events from the given View
	UFUNCTION(BlueprintCallable)
	void ListenToViewAction(UViewScreenComponent* InView);

	// set the transactor used
	UFUNCTION(BlueprintCallable)
	void SetTransactor(UActionScreenComponentProvider* InTransaction) { ActionProvider = InTransaction; }

protected:
	UFUNCTION()
	void HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget);

	// set the transactor that will be used
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Meta=(Displayname="Action"))
	TObjectPtr<UActionScreenComponentProvider> ActionProvider = nullptr;

	UPROPERTY()
	TObjectPtr<UViewScreenComponent> ViewListeningTo;

	// view screen component that we will list to events from
	UPROPERTY(EditAnywhere)
	FViewComponentSelector ViewToListenTo;

	// slots are provided as a holding place for data. So you can put it in a slot, and then have the action provider retrieve it later. 
	UPROPERTY()
	TMap<int32, UObject*> Slots;
};

