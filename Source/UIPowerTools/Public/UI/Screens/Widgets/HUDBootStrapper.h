// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBootStrapper.generated.h"

class UUserWidget;

/**
 * This actor can be set on your game instance and will add any screens in ScreensToDisplayAtStart
 * to the Screen Manager when BeginPlay occurs
 */
UCLASS()
class UIPOWERTOOLS_API AHUDBootStrapper : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void CreateScreensAndAddToManager();

	// will create and add these screens to manager when BeginPlay happens.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HudActor)
	TArray<TSubclassOf<UUserWidget>> ScreensToDisplayAtStart;
};
