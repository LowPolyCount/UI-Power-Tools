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
	virtual bool CanExecuteAction(UObject* Entry);

	// execute the transaction
	// @Component - the component calling the 
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - A Gameplay tag defining what the result was such as success, failure, etc. 
	UFUNCTION(BlueprintCallable, Category = ActionScreenComponentProvider)
	virtual bool ExecuteAction(UObject* Entry);


	//@note: Disabling until after 1.0 release
	/*UFUNCTION(BlueprintCallable)
	virtual FText GetTextAssociatedWithTag(const FGameplayTag& Tag) const;
	*/
	
	// Get the tag that contains 
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLastExecuteResultTag() const {return LastExecuteResultTag;}

protected:

	// Do we have all the information required to execute the transaction?
	// @Component - the component calling the function
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - Can we execute this action with the parameters given?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ActionScreenComponentProvider)
	bool CanExecuteActionInternal(UObject* Entry);

	// execute the transaction
	// @Component - the component calling the 
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - A Gameplay tag defining what the result was such as success, failure, etc. 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ActionScreenComponentProvider)
	bool ExecuteActionInternal(UObject* Entry);

	UFUNCTION(BlueprintCallable, Meta=(BlueprintProtected))
	void SetExecuteResultTag(const FGameplayTag Result) { LastExecuteResultTag = Result;}

	virtual bool CanExecuteActionInternal_Implementation(UObject* Entry);
	virtual bool ExecuteActionInternal_Implementation(UObject* Entry);

	// the Action Component that is the owner of this provider
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UActionScreenComponent> Owner;
	
	// a gameplay tag describing more exactly what happened during the last call made to CanExecuteAction() or ExecuteAction()
	FGameplayTag LastExecuteResultTag;

	// contains a mapping between the result of a query and Human Readable Text that can be displayed. 
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FText> ResultTagToText;
};
