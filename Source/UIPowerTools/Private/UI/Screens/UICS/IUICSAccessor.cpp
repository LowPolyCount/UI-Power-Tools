// Copyright (c) Joel Gonzales

#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UIPowerTools.h"
#include "Components/Button.h"
#include "UI/Screens/UICSScreen.h"

UScreenComponent* IUICSAccessor::GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	
	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		RetVal = Screen->GetScreenComponent_BP(Type);
	}
	return RetVal;
}

TArray<UScreenComponent*> IUICSAccessor::GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const
{
	TArray<UScreenComponent*> RetVal;
	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		RetVal = Screen->GetAllScreenComponents_BP(Type);
	}
	return RetVal;
}

UScreenComponent* IUICSAccessor::GetScreenComponentByName_BP(const FName Name, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		RetVal = Screen->GetScreenComponentByName_BP(Name, Type);
	}
	return RetVal;
}

UScreenComponent* IUICSAccessor::GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	if (TScriptInterface<IUICSScreenAccessor> Screen = GetScreenAccessor())
	{
		RetVal = Screen->GetScreenComponentFromSelector_BP(Selector, Type);
	}
	return RetVal;
}

TScriptInterface<IUICSScreenAccessor> IUICSAccessor::GetScreenAccessor() const
{
	TScriptInterface<IUICSScreenAccessor> RetVal = nullptr;
	if (const UObject* AsObject = Cast<UObject>(this))
	{
		UObjectBaseUtility* Outer = AsObject->GetImplementingOuterObject(UUICSScreenAccessor::StaticClass());
		RetVal = TScriptInterface<IUICSScreenAccessor>(Cast<UObject>(Outer));
		if (!RetVal)
		{
			UE_LOG(LogUICS, Warning, TEXT("Could not get owning Screen from %s - GetComponent() functions will not work"), *AsObject->GetFName().ToString());
		}
	}

	return RetVal;
}

UScreenComponent* IUICSScreenAccessor::GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		RetVal = ComponentManager->GetComponent(Type);
	}
	return RetVal;
}

TArray<UScreenComponent*> IUICSScreenAccessor::GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const
{
	TArray<UScreenComponent*> RetVal;
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		RetVal = ComponentManager->GetAllComponents(Type);
	}
	return RetVal;
}

UScreenComponent* IUICSScreenAccessor::GetScreenComponentByName_BP(const FName Name, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		RetVal = ComponentManager->GetComponentByName(Name, Type);
	}
	return RetVal;
}

UScreenComponent* IUICSScreenAccessor::GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		//@todo: Should we enforce Type here?
		RetVal = ComponentManager->GetComponentFromSelector(Selector);
	}
	return RetVal;
}

bool IUICSScreenAccessor::Initialize()
{
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		ComponentManager->Initialize();
	}

	return true;
}

void IUICSScreenAccessor::WidgetNativePreConstruct(bool bIsDesignTime)
{
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		ComponentManager->NativePreConstruct(bIsDesignTime);
	}
}

void IUICSScreenAccessor::NativeConstruct()
{
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		ComponentManager->NativeConstruct();
	}
}

void IUICSScreenAccessor::NativeDestruct()
{
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
		ComponentManager->NativeDestruct();
	}
}

#if WITH_EDITOR
UScreenComponent* IUICSScreenAccessor::GetScreenComponentFromGUID(const FGuid& Selector) const
{
	UScreenComponentManager* ComponentManager = GetComponentManager();
	return (ComponentManager) ? ComponentManager->GetComponentFromGUID(Selector) : nullptr;
}
#endif

UWidget* IUICSScreenAccessor::GetDesiredFocusTargetFromViewComponents() const
{
	UWidget* RetVal = nullptr;
	const TArray<UViewScreenComponent*> ViewComponents = IUICSAccessor::GetAllScreenComponents<UViewScreenComponent>();
	for (UViewScreenComponent* ViewComponent : ViewComponents)
	{
		if (ViewComponent->IsDesiredFocusTarget())
		{
			//@todo: Iterate through widgets and pick the first one that is Focusable
			const TArray<TScriptInterface<IViewWidgetInterface>>& ViewWidgets = ViewComponent->GetAllViewWidgets();
			for (TScriptInterface<IViewWidgetInterface> ViewWidget : ViewWidgets)
			{
				if (UUserWidget* AsUWidget = Cast<UUserWidget>(ViewWidget.GetObject()))
				{
					if (AsUWidget->IsFocusable())
					{
						RetVal = AsUWidget;
						break;
					}
				}
			}
		}
	}
	return RetVal;
}
