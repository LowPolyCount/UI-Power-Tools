// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/ScreenComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "GameplayTagContainer.h"
#include "Input/UIActionBindingHandle.h"
#include "InputScreenComponent.generated.h"

class APlayerController;
class UInputMappingContext;
class UInputAction;

// used to define Input Bindings
USTRUCT(BlueprintType, Experimental)
struct FUIPTInputBinding
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Meta=(EditCondition="CommonInput.CommonInputSettings.IsEnhancedInputSupportEnabled"))
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(BlueprintReadWrite)
	bool bDisplayedInActionBar = false;
};

UENUM(BlueprintType)
enum class EScreenInputMode : uint8
{
	None,
	// input from both player controller and UI
	GameAndUI,
	// input from player controller only
	GameOnly,
	// input from UI only
	UIOnly,
};

/**
 * controls options on how a screen handles Input
 */
UCLASS()
class UIPOWERTOOLS_API UInputScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:
	UInputScreenComponent();
	// UScreenComponent
	virtual void NativeConstruct() override;
	// UScreenComponent End


	UFUNCTION(BlueprintCallable)
	void SetInputMode(EScreenInputMode Mode) { ScreenInputMode = Mode;}
	UFUNCTION(BlueprintCallable)
	EScreenInputMode GetInputMode() const {return ScreenInputMode;}

	UFUNCTION(BlueprintCallable)
	void SetInputMapping(UInputMappingContext* InMappingContext) { MappingContext = InMappingContext;}

	UFUNCTION(BlueprintCallable)
	UInputMappingContext* GetInputMapping() const {return MappingContext;}

	FUIInputConfig GetInputConfig() const {return InputConfig;}

	// no copy constructor on FUIInputConfig
	//const FUIInputConfig& SetInputConfig(FUIInputConfig Config) const { InputConfig = Config; }


	void SetMouseLockMode(EMouseLockMode LockMode) { MouseLockMode = LockMode; }
	void SetHideCursorDuringCapture(bool bHideCursor) { bHideCursorDuringCapture = bHideCursor; }
	void SetFlushInput(bool bFlush) {  bFlushInput = bFlush; }
	EMouseLockMode GetMouseLockMode() const {return MouseLockMode;}
	bool GetHideCursorDuringCapture() const {return bHideCursorDuringCapture;}
	bool GetFlushInput() const {return bFlushInput;}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIInputConfig InputConfig;

	// how the mouse should work when clicking on the viewport. See EMouseCaptureMode in 
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//EMouseCaptureMode MouseCaptureMode = EMouseCaptureMode::CapturePermanently;

	//@note: InputMode is set by the ScreenManagerSubsystem because we have to keep track of what mode we're in. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EScreenInputMode ScreenInputMode = EScreenInputMode::UIOnly;

	// how the mouse should lock to the viewport
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (EditCondition = "ScreenInputMode==(EScreenInputMode::UIOnly || EScreenInputMode::GameAndUI)"))
	EMouseLockMode MouseLockMode = EMouseLockMode::DoNotLock;

	// When captured, is the mouse cursor hidden?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(EditCondition="ScreenInputMode==EScreenInputMode::GameAndUI"))
	bool bHideCursorDuringCapture = false;

	// when changing input modes, is the input flushed?
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFlushInput = false;
};
