// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/EnumAsByte.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "QuitGameActionProvider.generated.h"

enum class EActionResult : uint8;

/**
 * An Action used by the Action Screen Component that will quit the game.
 */
UCLASS()
class UIPOWERTOOLS_API UQuitGameActionProvider : public UActionScreenComponentProvider
{
	GENERATED_BODY()
protected:
	virtual EActionResult ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry) override;

	// Quit application or move to background?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ActionProvider)
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Type::Quit;

	// Ignore Platform Restrictions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ActionProvider)
	bool bIgnorePlatformRestrictions = false;
};
