// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDActor.generated.h"

class UScreen;
class UUserWidget;
class UScreenManager;

/**
 * The HUD actor is defined on the game instance and can be used to bootstrap the UI
 * Will add the defined ScreensToDisplayAtStart to the Screen Manager when BeginPlay occurs
 */
UCLASS()
class UIPOWERTOOLS_API AHUDActor : public AHUD
{
	GENERATED_BODY()

public:
	AHUDActor(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// use UScreenManager::Get() to retrieve the ScreenManager in Blueprint
	UScreenManager* GetScreenManager() const { return ScreenManager; }

protected:
	UFUNCTION()
	virtual void CreateScreensAndAddToManager();

	// will create and add these screens to manager when BeginPlay happens.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HudActor)
	TArray<TSubclassOf<UUserWidget>> ScreensToDisplayAtStart;

	UPROPERTY()
	TObjectPtr<UScreenManager> ScreenManager;
};
