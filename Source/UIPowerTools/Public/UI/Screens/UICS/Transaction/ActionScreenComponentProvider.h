// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionScreenComponentProvider.generated.h"

enum class ETransactionResult : uint8;

// Transactions make changes to our data and can query if we're in the correct state to start the transaction
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UActionScreenComponentProvider : public UObject
{
	GENERATED_BODY()
public:
	// Do we have all the information required to execute the transaction?
	UFUNCTION(BlueprintNativeEvent)
	bool CanExecuteAction(UActionScreenComponent* Component, UObject* Entry);

	// execute the transaction
	UFUNCTION(BlueprintNativeEvent)
	ETransactionResult ExecuteAction(UActionScreenComponent* Component, UObject* Entry);

protected:
	virtual ETransactionResult ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry);
	virtual bool CanExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry) { return true; }
};
