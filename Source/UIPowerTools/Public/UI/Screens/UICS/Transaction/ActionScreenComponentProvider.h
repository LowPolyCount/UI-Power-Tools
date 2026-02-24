// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "NativeGameplayTags.h"
#include "ActionScreenComponentProvider.generated.h"

// Transactions make changes to our data and can query if we're in the correct state to start the transaction
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UActionScreenComponentProvider : public UScreenComponentWorldContext, public IUICSAccessor
{
	GENERATED_BODY()
public:
	// Do we have all the information required to execute the transaction?
	// @Component - the component calling the function
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - Can we execute this action with the parameters given?
	UFUNCTION(BlueprintNativeEvent, Category = ActionScreenComponent)
	bool CanExecuteAction(UActionScreenComponent* Component, UObject* Entry);

	// execute the transaction
	// @Component - the component calling the 
	// @Entry - Data (usually) from the currently focused/selected widget 
	// @Return - A Gameplay tag defining what the result was such as success, failure, etc. 
	UFUNCTION(BlueprintNativeEvent, Category = ActionScreenComponent)
	FGameplayTag ExecuteAction(UActionScreenComponent* Component, UObject* Entry);

protected:
	virtual FGameplayTag ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry);
	virtual bool CanExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry) { return true; }
};
