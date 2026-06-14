// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "NativeGameplayTags.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "ActionScreenComponent.generated.h"

class UViewScreenComponent;
class UActionScreenComponentProvider;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValidTransactionResult, UActionScreenComponent*, Component, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FActionResult, UActionScreenComponent*, Component, bool, bResult, const FGameplayTag&, Result);

// FTransactionResult is deprecated and will be removed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTransactionResult, UActionScreenComponent*, Component, const FGameplayTag&, Result);

//Gameplay tags that define what the outcome of an action was
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_ACTION_Default);			// Default Value
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_Action_CouldNotExecute);	// ExecuteActionIfAble() was called and The Action provider's CanExecuteAction() returned false
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_Action_Success);			// ExecuteAction() was called and action was executed successfully
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_Action_Async);				// Action is waiting for an asynchronous callback
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_Action_Failure);			// ExecuteAction() Was called, but not able to execute
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_ACTION_NoActionComponent);	// There is no Action Component linked to a View Component. (Can happen for IViewWidgets)
UIPOWERTOOLS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_ACTION_NoActionProvider);	// There is no action provider

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct UIPOWERTOOLS_API FBindableActionEvents
{
	GENERATED_BODY()
	// we called ExecuteAction() - what was the result?
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ActionScreenComponent.Binding_ActionExecuteResult", DefaultBindingName="ActionExecuteResult", DisplayName = "OnActionExecuteResult"))
	FMemberReference  Bind_OnActionExecuteResult;
};

// Action Component collects all the information required to execute an "Action". Examples are, opening a screen or buying an item.
UCLASS(AutoExpandCategories = Triggers)
class UIPOWERTOOLS_API UActionScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
protected:

	void Initialize() override;

public:
	// result of ExecuteAction
	UPROPERTY(BlueprintAssignable, Category = ActionScreenComponent)
	FActionResult OnActionExecuteResult;


	// Given the Entry Data, can we execute the current action?
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool CanExecuteAction(UObject* Entry = nullptr);

	// call CanExecuteAction() and if true, then call ExecuteAction(). If False, return CouldNotExecute
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool ExecuteActionIfAble(UObject* Entry = nullptr);

	// execute our action, without checking CanExecuteAction()
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool ExecuteAction(UObject* Entry = nullptr);

	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	FGameplayTag GetLastActionResult() const;

	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool HasTextAssociatedWithLastActionResultTag() const;

	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	FText GetTextAssociatedWithLastActionResultTag() const;

	// set the action provider
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetActionProvider(UActionScreenComponentProvider* InActionProvider) { ActionProvider = InActionProvider; }

	// get the action provider
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	UActionScreenComponentProvider* GetActionProvider() const {return ActionProvider;}

	// listen to events from the given screen component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void ListenToViewScreenComponent(UViewScreenComponent* InView);

	// Get if we will trigger on a Input Action Event from a widget with the Linked View Screen Component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool GetTriggerOnInputAction() const { return bTriggerOnInputAction; }

	// Get if we will trigger on a Hover Event from a widget with the Linked View Screen Component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool GetTriggerOnHover() const { return bTriggerOnHover; }

	// Get if we will trigger on a Gain Focus Event from a widget with the Linked View Screen Component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	bool GetTriggerOnGainsFocus() const { return bTriggerOnGainsFocus; }

	// Set if we will trigger on a Input Action Event from a widget with the Linked View Screen Component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetTriggerOnGainsFocus(bool bInTriggerOnGainsFocus);

	// Set if we will trigger on a Hover Event from a widget with the Linked View Screen Component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetTriggerOnHover(bool bInTriggerOnHover);

	// Set if we will trigger on a Gain Focus Event from a widget with the Linked View Screen Component
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent)
	void SetTriggerOnInputAction(bool bInTriggerOnInputAction);

protected:
	UFUNCTION()
	void HandleOnActionTrigger(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);
	UFUNCTION()
	void HandleOnActionTriggerGain(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained);

	void RemoveCurrentViewScreenComponent();
	void SetupListenersToViewScreenComponent(UViewScreenComponent* InView);

	// The action provider implements the action that you want to take place
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = ActionScreenComponent, Meta=(Displayname="Action"))
	TObjectPtr<UActionScreenComponentProvider> ActionProvider = nullptr;

	// Will execute our action in response to the view screen component chosen
	UPROPERTY(EditAnywhere, Category = ActionScreenComponent)
	FViewComponentSelector ViewToListenTo;

	//@note: Tried using a Bitmask instead of individual booleans, but there is some type of issue in the editor where the value
	//		is not being set correctly as of 7.4

	// Trigger the action when an Input Action on a widget with the Linked View Screen Component happens.
	UPROPERTY(EditAnywhere, Category=Triggers)
	bool bTriggerOnInputAction = true;

	// Trigger the action when a Hover Event on a widget with the Linked View Screen Component happens
	UPROPERTY(EditAnywhere, Category=Triggers)
	bool bTriggerOnHover = false;

	// Trigger the action when a Gains Focus Event on a widget with the Linked View Screen Component happens
	UPROPERTY(EditAnywhere, Category=Triggers)
	bool bTriggerOnGainsFocus = false;

	UPROPERTY(EditAnywhere, Category = ActionScreenComponent, Meta = (DisplayName = "Events"));
	FBindableActionEvents BindableEvents;

	UPROPERTY()
	TObjectPtr<UViewScreenComponent> ViewListeningTo;

	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly, Meta=(DeprecatedFunction, DeprecationMessage="Use Binding_ActionExecuteResult()"))
	void Binding_TransactionResult(UActionScreenComponent* Component, const FGameplayTag& Result) {}
	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_IsTransactionValid(UActionScreenComponent* Component, bool bResult) {}

	UFUNCTION(BlueprintInternalUseOnly)
	void Binding_ActionExecuteResult(UActionScreenComponent* Component, bool bDidExecuteSucceed, const FGameplayTag& Result) {}
#endif // WITH_EDITOR
	// END FMember References

	

public:
	// result of ExecuteAction
	UE_DEPRECATED(Any, "is deprecated. Use OnActionExecuteResult instead")
	UPROPERTY(BlueprintAssignable, Category = ActionScreenComponent, Meta = (DeprecatedFunction, DeprecationMessage = "Use OnActionExecuteResult instead"))
	FTransactionResult OnExecuteResult_DEPRECATED;

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

		// listen for input events from the given View
	UE_DEPRECATED(Any, "Is Deprecated. Use ListenToViewScreenComponent() Instead")
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponent, meta = (DeprecatedFunction, DeprecationMessage = "HandleOnAction is deprecated. Use ListenToViewScreenComponent instead"))
	void ListenToViewAction(UViewScreenComponent* InView);

private:

	// slots are provided as a holding place for data. So you can put it in a slot, and then have the action provider retrieve it later. 
	UPROPERTY()
	TMap<int32, UObject*> Slots;


};

// deprecated
// defines the results of a transaction
UENUM(BlueprintType)
enum class ETransactionResult : uint8
{
	// ExecuteActionIfAble() was called and The Action provider's CanExecuteAction() returned false
	CouldNotExecute,	
	Success,
	Failure,
	// The result is waiting on an Asynchronous call to complete
	Async			
};