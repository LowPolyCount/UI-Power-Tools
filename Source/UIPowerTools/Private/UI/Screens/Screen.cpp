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

	/*if (const UUIPowerToolsDeveloperSettings* DefaultSettings = GetDefault<UUIPowerToolsDeveloperSettings>())
	{
		if (UInputAction* DefaultCancelAction = Cast<UInputAction>(DefaultSettings->DefaultCancelAction.TryLoad()))
		{
			CancelAction.InputAction = DefaultCancelAction;
			CancelAction.bDisplayedInActionBar = false;
		}
	}*/

}

TOptional<FUIInputConfig> UScreen::GetDesiredInputConfig() const
{
	// Check if there is a BP implementation for input configs
	// we have to use the FName because we don't have access to the class function
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetDesiredInputConfig")))
	{
		return Super::GetDesiredInputConfig();
	}
	else if (const UInputScreenComponent* InputScreenComponent = GetScreenComponent<UInputScreenComponent>())
	{
		return InputScreenComponent->GetInputConfig();
	}

	return Super::GetDesiredInputConfig();
}

UWidget* UScreen::NativeGetDesiredFocusTarget() const
{
	UWidget* RetVal = nullptr;

	// we have to use the FName because we don't have access to the class function
	if (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetDesiredFocusTarget")))
	{
		RetVal = Super::NativeGetDesiredFocusTarget();
	}

	if (!RetVal)
	{
		const TArray<UViewScreenComponent*> ViewComponents = GetAllScreenComponents<UViewScreenComponent>();
		for (UViewScreenComponent* ViewComponent : ViewComponents)
		{
			if (ViewComponent->IsDesiredFocusTarget())
			{
				if (TScriptInterface<IEntryWidgetInterface> AsViewWidget = ViewComponent->GetViewWidgetAt(0))
				{
					//@todo: Iterate through widgets and pick the first one that is Focusable
					/*const TArray<TScriptInterface<IEntryWidgetInterface>>& ViewWidgets = ViewComponent->GetAllViewWidgets();
					for (TScriptInterface<IEntryWidgetInterface> ViewWidget : ViewWidgets)
					{
						ViewWidget->Execute_IsFocusable(ViewWidget.GetObject());
					}
					*/

					if (UWidget* AsWidget = Cast<UWidget>(AsViewWidget.GetObject()))
					{
						RetVal = AsWidget;
						break;
					}
				}
			}
		}
	}

	if (!RetVal)
	{
		return Super::NativeGetDesiredFocusTarget();
	}
	
	return RetVal;
}

bool UScreen::Initialize()
{
	const bool bRetVal = Super::Initialize();

	if (ComponentManager)
	{
		ComponentManager->Initialize();
	}

	if (!IsDesignTime())
	{
		if (UInputScreenComponent* ISComponent = GetScreenComponent<UInputScreenComponent>())
		{
			InputMapping = ISComponent->GetInputMapping();
		}
	}

	return bRetVal;
}

void UScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (ComponentManager)
	{
		ComponentManager->NativePreConstruct(IsDesignTime());
	}

	if (!IsDesignTime())
	{
		if (UInputScreenComponent* ISComponent = GetScreenComponent<UInputScreenComponent>())
		{
			InputMapping = ISComponent->GetInputMapping();
		}
	}
}

void UScreen::NativeConstruct()
{
	Super::NativeConstruct();
	if (ComponentManager)
	{
		ComponentManager->NativeConstruct();
	}

	/*if (UCommonInputSettings::IsEnhancedInputSupportEnabled())
	{
		FBindUIActionArgs BindArgs(CancelAction.InputAction, FSimpleDelegate::CreateUObject(this, &UScreen::HandleClose));
		BindArgs.bDisplayInActionBar = CancelAction.bDisplayedInActionBar;

		RegisterUIActionBinding(BindArgs);
	}*/

	if (UCommonInputSettings::IsEnhancedInputSupportEnabled() && InputMapping)
	{
		if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputSystem->AddMappingContext(InputMapping, InputMappingPriority);
			}
		}
	}
}

/*void UScreen::HandleClose()
{
	Close();
}*/

void UScreen::NativeDestruct()
{
	if (ComponentManager)
	{
		ComponentManager->NativeDestruct();
	}
	Super::NativeDestruct();
}

void UScreen::BeginDestroy()
{
	if (ComponentManager)
	{
		ComponentManager->ConditionalBeginDestroy();
	}
	Super::BeginDestroy();
}

#if WITH_EDITOR
UScreenComponent* UScreen::GetScreenComponentFromGUID(const FGuid& Selector) const
{
	return (ComponentManager) ? ComponentManager->GetComponentFromGUID(Selector) : nullptr;
}
#endif

UScreenComponent* UScreen::GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	if (ComponentManager)
	{
		RetVal = ComponentManager->GetComponent(Type);
	}
	return RetVal;
}

TArray<UScreenComponent*> UScreen::GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const
{
	TArray<UScreenComponent*> RetVal;
	if (ComponentManager)
	{
		RetVal = ComponentManager->GetAllComponents(Type);
	}
	return RetVal;
}

UScreenComponent* UScreen::GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	if (ComponentManager)
	{
		RetVal = ComponentManager->GetComponentFromSelector(Selector);
	}
	return RetVal;
}
