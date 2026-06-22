// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "NativeGameplayTags.h"
#include "ActionScreenComponentProvider.generated.h"

class UActionScreenComponent;

// Transactions make changes to our data and can query if we're in the correct state to start the transaction
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UActionScreenComponentProvider : public UScreenComponentWorldContext
{
	GENERATED_BODY()
public:
	// initialize the provider. Should be called before any other functions. 
	void Initialize(UActionScreenComponent* InOwner);

	// Determine if the data given can currently be executed by the Action Provider
	// @Component - the component calling the function
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - Can we execute this action with the parameters given?
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponentProvider)
	virtual bool NativeCanExecuteAction(UObject* Entry);

	// execute the transaction
	// @Component - the component calling the 
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - A Gameplay tag defining what the result was such as success, failure, etc. 
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponentProvider)
	virtual bool NativeExecuteAction(UObject* Entry);

	// Do we have text associated with the current last action result tag?
	UFUNCTION(BlueprintCallable)
	bool HasTextAssociatedWithLastActionResultTag() const;

	// returns the text associated with the last action result tag that was set when either CanExecuteAction() or ExecuteAction() were called.
	// @return - Text from Map association.  Will be empty if no text is associated with the last action result tag. 
	UFUNCTION(BlueprintCallable)
	FText GetTextAssociatedWithLastActionResultTag() const;

	
	// Get the tag that contains 
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLastActionResult() const {return LastActionResult;}

protected:

	// Do we have all the information required to execute the transaction?
	// @note - Implementor Should set LastActionResult as part of implementation
	// @Entry - Data (usually) from a widget created by a View Screen Component linked to the owning Action Screen Component
	// @Return bool - true if we can we execute this action with the parameters given
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = ActionScreenComponentProvider, Meta=(DisplayName="Can Execute Action"))
	bool BP_CanExecuteAction(UObject* Entry);

	// execute the transaction
	// @note - Implementor Should set LastActionResult as part of implementation
	// @Entry - Data (usually) from a widget created by a View Screen Component linked to the owning Action Screen Component
	// @Return - true if we executed the action successfully
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = ActionScreenComponentProvider, Meta = (DisplayName = "Execute Action"))
	bool BP_ExecuteAction(UObject* Entry);

	UFUNCTION(BlueprintCallable, Meta=(BlueprintProtected, GameplayTagFilter = "UICS.Action"))
	void SetActionResult(const FGameplayTag Result) { LastActionResult = Result;}

	// the Action Component that is the owner of this provider
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActionScreenComponent> Owner;
	
	// a gameplay tag describing more exactly what happened during the last call made to CanExecuteAction() or ExecuteAction()
	FGameplayTag LastActionResult;

	// contains a mapping between the result of a query and Human Readable Text that can be displayed. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FText> ActionResultTagToText;

public:

	// Do we have all the information required to execute the transaction?
	// @note - Implementor Should set LastActionResult as part of implementation
	// @Entry - Data (usually) from a widget created by a View Screen Component linked to the owning Action Screen Component
	// @Return bool - true if we can we execute this action with the parameters given
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ActionScreenComponentProvider, Meta=(DeprecatedFunction, DeprecationMessage="Use CanExecuteAction"))
	bool CanExecuteActionInternal(UObject* Entry);

	// execute the transaction
	// @note - Implementor Should set LastActionResult as part of implementation
	// @Entry - Data (usually) from a widget created by a View Screen Component linked to the owning Action Screen Component
	// @Return - true if we executed the action successfully
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ActionScreenComponentProvider, Meta=(DeprecatedFunction, DeprecationMessage="Use ExecuteAction"))
	bool ExecuteActionInternal(UObject* Entry);

	UE_DEPRECATED(Any, "Is Deprecated. Override NativeExecuteAction() Instead")
	virtual bool ExecuteActionInternal_Implementation(UObject* Entry) {return false;}
	UE_DEPRECATED(Any, "Is Deprecated. Override NativeExecuteAction() Instead")
	virtual bool CanExecuteActionInternal_Implementation(UObject* Entry) {return false;}
};
