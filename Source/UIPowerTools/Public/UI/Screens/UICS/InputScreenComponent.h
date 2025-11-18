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
	UPROPERTY(EditAnywhere, Category = InputScreenComponent, Meta=(EditCondition="CommonInput.CommonInputSettings.IsEnhancedInputSupportEnabled"))
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputScreenComponent)
	bool bDisplayedInActionBar = true;
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
UCLASS(Experimental)
class UIPOWERTOOLS_API UInputScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:
	//UInputScreenComponent();
	// UScreenComponent
	virtual void NativeConstruct() override;
	// UScreenComponent End


	UFUNCTION(BlueprintCallable, Category = InputScreenComponent)
	void SetInputMode(EScreenInputMode Mode) { ScreenInputMode = Mode;}
	UFUNCTION(BlueprintCallable, Category = InputScreenComponent)
	EScreenInputMode GetInputMode() const {return ScreenInputMode;}

	UFUNCTION(BlueprintCallable, Category = InputScreenComponent)
	void SetInputMapping(UInputMappingContext* InMappingContext) { MappingContext = InMappingContext;}

	UFUNCTION(BlueprintCallable, Category = InputScreenComponent)
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputScreenComponent)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputScreenComponent)
	FUIInputConfig InputConfig;

	// how the mouse should work when clicking on the viewport. See EMouseCaptureMode in 
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//EMouseCaptureMode MouseCaptureMode = EMouseCaptureMode::CapturePermanently;

	//@note: InputMode is set by the ScreenManagerSubsystem because we have to keep track of what mode we're in. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputScreenComponent)
	EScreenInputMode ScreenInputMode = EScreenInputMode::UIOnly;

	// how the mouse should lock to the viewport
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputScreenComponent, Meta = (EditCondition = "ScreenInputMode==(EScreenInputMode::UIOnly || EScreenInputMode::GameAndUI)"))
	EMouseLockMode MouseLockMode = EMouseLockMode::DoNotLock;

	// When captured, is the mouse cursor hidden?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputScreenComponent, Meta=(EditCondition="ScreenInputMode==EScreenInputMode::GameAndUI"))
	bool bHideCursorDuringCapture = false;

	// when changing input modes, is the input flushed?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputScreenComponent)
	bool bFlushInput = false;
};
