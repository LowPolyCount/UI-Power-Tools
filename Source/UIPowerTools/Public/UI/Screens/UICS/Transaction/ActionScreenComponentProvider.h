// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "ActionScreenComponentProvider.generated.h"

enum class ETransactionResult : uint8;

// Transactions make changes to our data and can query if we're in the correct state to start the transaction
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class UIPOWERTOOLS_API UActionScreenComponentProvider : public UScreenComponentWorldContext, public IUICSAccessor
{
	GENERATED_BODY()
public:
	// Do we have all the information required to execute the transaction?
	UFUNCTION(BlueprintNativeEvent, Category = ActionScreenComponent)
	bool CanExecuteAction(UActionScreenComponent* Component, UObject* ViewWidgetData);

	// execute the transaction
	// WidgetEntry - this is data coming from a view widget. 
	UFUNCTION(BlueprintNativeEvent, Category = ActionScreenComponent)
	ETransactionResult ExecuteAction(UActionScreenComponent* Component, UObject* ViewWidgetData);

protected:
	virtual ETransactionResult ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry);
	virtual bool CanExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry) { return true; }
};
