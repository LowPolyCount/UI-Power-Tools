// Copyright (c) Joel Gonzales


#include "UI/Screens/Screen.h"
#include "InputAction.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
#include "UI/Screens/UICS/InputScreenComponent.h"
#include "UI/Tools/UIPowerToolsDeveloperSettings.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Input/CommonUIInputTypes.h"
#include "CommonInputSettings.h"
#include "Input/UIActionBinding.h"



UScreen::UScreen(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
	ComponentManager = Initializer.CreateDefaultSubobject<UScreenComponentManager>(this, TEXT("ScreenComponentManager"));
}

TOptional<FUIInputConfig> UScreen::GetDesiredInputConfig() const
{
	// input testing, not ready for production
	/* // Check if there is a BP implementation for input configs
	// we have to use the FName because we don't have access to the class function
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetDesiredInputConfig")))
	{
		return Super::GetDesiredInputConfig();
	}
	else if (const UInputScreenComponent* InputScreenComponent = GetScreenComponent<UInputScreenComponent>())
	{
		return InputScreenComponent->GetInputConfig();
	}

	return Super::GetDesiredInputConfig();*/
	return Super::GetDesiredInputConfig();
}

UWidget* UScreen::NativeGetDesiredFocusTarget() const
{
	// if we haven't overridden this in BP, then get the focus from the view component
	return (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetDesiredFocusTarget")))
		? Super::NativeGetDesiredFocusTarget()
		: IUICSScreenAccessor::GetDesiredFocusTargetFromViewComponents();
}

bool UScreen::Initialize()
{
	const bool bRetVal = Super::Initialize();
	
	IUICSScreenAccessor::Initialize();

	// input testing, not ready for production
	/*if (!IsDesignTime())
	{
		if (UInputScreenComponent* ISComponent = GetScreenComponent<UInputScreenComponent>())
		{
			InputMapping = ISComponent->GetInputMapping();
		}
	}*/

	return bRetVal;
}

void UScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
	IUICSScreenAccessor::WidgetNativePreConstruct(IsDesignTime());

	// input testing, not ready for production
	/*if (!IsDesignTime())
	{
		if (UInputScreenComponent* ISComponent = GetScreenComponent<UInputScreenComponent>())
		{
			InputMapping = ISComponent->GetInputMapping();
		}
	}*/
}

void UScreen::NativeConstruct()
{
	Super::NativeConstruct();
	IUICSScreenAccessor::NativeConstruct();

	// input testing, not ready for production
	/*if (UCommonInputSettings::IsEnhancedInputSupportEnabled() && InputMapping)
	{
		if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputSystem->AddMappingContext(InputMapping, InputMappingPriority);
			}
		}
	}*/
}

void UScreen::NativeDestruct()
{
	IUICSScreenAccessor::NativeDestruct();
	Super::NativeDestruct();
}
