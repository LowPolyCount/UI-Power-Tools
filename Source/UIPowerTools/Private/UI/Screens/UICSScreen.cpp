// Copyright (c) Joel Gonzales


#include "UI/Screens/UICSScreen.h"
#include "InputAction.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
#include "UI/Tools/UIPowerToolsDeveloperSettings.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Input/CommonUIInputTypes.h"
#include "CommonInputSettings.h"
#include "Input/UIActionBinding.h"



UUICSScreen::UUICSScreen(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
	ComponentManager = Initializer.CreateDefaultSubobject<UScreenComponentManager>(this, TEXT("ScreenComponentManager"));
}

UWidget* UUICSScreen::NativeGetDesiredFocusTarget() const
{
	// if we haven't overridden this in BP, then get the focus from the view component
	return (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetDesiredFocusTarget")))
		? Super::NativeGetDesiredFocusTarget()
		: IUICSScreenAccessor::GetDesiredFocusTargetFromViewComponents();
}

bool UUICSScreen::Initialize()
{
	const bool bRetVal = Super::Initialize();
	
	IUICSScreenAccessor::Initialize();

	return bRetVal;
}

void UUICSScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
	IUICSScreenAccessor::WidgetNativePreConstruct(IsDesignTime());
}

void UUICSScreen::NativeConstruct()
{
	Super::NativeConstruct();
	IUICSScreenAccessor::NativeConstruct();
}

void UUICSScreen::NativeDestruct()
{
	IUICSScreenAccessor::NativeDestruct();
	Super::NativeDestruct();
}
