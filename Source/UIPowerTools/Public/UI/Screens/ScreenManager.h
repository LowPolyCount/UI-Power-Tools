// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "ScreenManager.generated.h"

class UOverlay;
class UScreen;
class UCommonActivatableWidget;
enum class EScreenInputMode : uint8;

// this struct contains everything needed to handle changes when screens are added or closed
USTRUCT(Blueprintable, BlueprintType)
struct UIPOWERTOOLS_API FScreenStruct
{
	GENERATED_BODY()
public:
	FScreenStruct() {}
	FScreenStruct(UUserWidget* InScreen, bool bHideScreensBelow = true);
	bool operator==(const FScreenStruct& rhs) const { return Screen == rhs.Screen; }
	void SetVisibility(bool bIsVisible);
	void SetEnableInput(bool bEnableInput);

	// the screen being added
	UPROPERTY(BlueprintReadWrite, Category = ScreenManager)
	TObjectPtr<UUserWidget> Screen;

	// status of what all screens 
	UPROPERTY(BlueprintReadWrite, Category = ScreenManager)
	bool bHideScreensBelow = true;
};

/**
 * A Manager for displaying screens
 * @note: Screen Manager handles screen visibility. So if the user manually sets the screen's visibility, the Manager can override it when a screen is added or removed. 
 */
UCLASS(BlueprintType)
class UIPOWERTOOLS_API UScreenManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReadyDelegate);

	UScreenManager();

	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type Reason);
	
	// Get the screen manager
	UFUNCTION(BlueprintPure, Category = ScreenManager, Meta=(DisplayName="Get ScreenManager", WorldContext = "WorldContextObject"))
	static UScreenManager* Get(const UObject* WorldContextObject);

	// broadcasts when the subsystem is ready to accept screens
	UPROPERTY(BlueprintAssignable, Category = ScreenManager)
	FReadyDelegate OnReady;

	// this only needs to be checked when first starting a game instance, such as with the Hud Actor
	// this is because the subsystem can be created before the Viewport, so we need to wait on that before
	// something can be added.
	UFUNCTION(BlueprintCallable, Category = ScreenManager)
	bool IsReady() const;

	// add a screen to the manager to be displayed
	// to remove a screen, call the screen's Close()
	// @bHideScreensBelow = Are all existing displayed screens hidden when this screen is added?
	UFUNCTION(BlueprintCallable, Category = ScreenManager)
	void AddScreen(UUserWidget* Screen, bool bHideScreensBelow = true);

	// *Only Added for convenience*
	// Instead of calling this, call the UUserWidget's RemoveFromParent()
	void RemoveScreen(UUserWidget* Screen);

	// is the given screen instance on the stack?
	UFUNCTION(BlueprintCallable, Category = ScreenManager)
	bool IsScreenOnStack(UUserWidget* Screen) const;

	// is an instance of a screen of the given class on the stack?
	UFUNCTION(BlueprintCallable, Category = ScreenManager)
	bool IsScreenOfClassOnStack(TSubclassOf<UUserWidget> Class) const;

	// number of screens being displayed
	UFUNCTION(BlueprintCallable, Category = ScreenManager)
	int32 NumScreens() const {return Screens.Num();}

	// get the screen at the top of the stack
	UFUNCTION(BlueprintCallable, Category = ScreenManager)
	UUserWidget* GetScreenOnTop() const;

protected:
	
	UFUNCTION()
	void HandleOnNativeDestruct(UUserWidget* Screen);

	UFUNCTION()
	void HandleOnAddToViewport();
	UFUNCTION()
	void HandleOnReady();

	void AddScreen_Internal(FScreenStruct& ScreenToAdd);
	void AddScreenUsingPanel(FScreenStruct& ScreenToAdd);
	void AddScreenUsingViewport(FScreenStruct& ScreenToAdd);
	void RemoveScreenUsingPanel(UUserWidget* Screen);
	void RemoveScreenUsingViewport(UUserWidget* Screen);
	virtual APlayerController* GetPlayerController();
	//virtual void SetInputMode(APlayerController* PlayerController, const UInputScreenComponent* InputComponent);
	void ActivateTopScreen();

	//EScreenInputMode CurrentInputMode;

	UPROPERTY()
	TArray<FScreenStruct> Screens;
	int32 ZValue = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UUserWidget> RootWidget;
	UPROPERTY()
	TObjectPtr<UOverlay> ParentPanel;

	FTimerHandle TimerHandle;

	bool bRootAddedToViewport = false;
};
