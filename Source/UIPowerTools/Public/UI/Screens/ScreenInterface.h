// Copyright (c) 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ScreenInterface.generated.h"


// a generic delegate that a screen can broadcast
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScreenDelegate, TScriptInterface<IScreenInterface>, Screen);


UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * We use an interface to define screen functionality that the Screen Manager has access to. 
 * This is so that any UserWidget you are working with can be defined as a screen usable with the Screen Manager
 */
class UIPOWERTOOLS_API IScreenInterface
{
	GENERATED_BODY()
public:
	bool IsOnScreenManager() const {return false;}

	virtual void AddScreenToViewport(int32 ZOrder);
	
	// provided as utility function 
	UUserWidget* AsUserWidget();

	// Input is enabled by assuming the underlying derived widget inherits from UCommonActivatableWidget
	virtual void SetInputEnabled(bool bEnabled);

	bool GetInputEnabled(bool bEnabled) const {return bInputEnabled;}

	// close this screen.
	// note: If you override this, do everything needed before calling Super::Close() as that will
	//			cause the ScreenManager to remove this screen from the viewport. 
	UFUNCTION(BlueprintCallable, Category = Screen)
	virtual void Close();

	// called when the screen is closed. 
	virtual FScreenDelegate& GetOnScreenClose() = 0;

protected:
	bool bInputEnabled = false;
};
