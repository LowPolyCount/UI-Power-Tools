// Copyright (c) Joel Gonzales


#include "UI/Screens/Screen.h"
#include "InputAction.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
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

	return bRetVal;
}

void UScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
	IUICSScreenAccessor::WidgetNativePreConstruct(IsDesignTime());
}

void UScreen::NativeConstruct()
{
	Super::NativeConstruct();
	IUICSScreenAccessor::NativeConstruct();
}

void UScreen::NativeDestruct()
{
	IUICSScreenAccessor::NativeDestruct();
	Super::NativeDestruct();
}
