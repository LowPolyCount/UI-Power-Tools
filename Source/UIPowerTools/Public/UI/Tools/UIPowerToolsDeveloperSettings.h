// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UIPowerToolsDeveloperSettings.generated.h"

/**
 * Developer Settings for UI Power Tools
 */
UCLASS(Config=Game, DefaultConfig, Meta=(DisplayName="UI Power Tools"))
class UIPOWERTOOLS_API UUIPowerToolsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//Begin UDeveloperSettings interface
	virtual FName GetCategoryName() const;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif
	//End

	// when screen is added to the screen manager, what number is used as the starting z-order when it gets added to the viewport?
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Meta=(ClampMin=0))
	int32 ScreenManagerZOrderStart = 100;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Meta=(AllowedClasses="/Script/EnhancedInput.InputAction"))
	FSoftObjectPath DefaultCancelAction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Meta=(AllowedClasses="/Script/EnhancedInput.InputMappingContext"))
	FSoftObjectPath DefaultInputMapping;
};
