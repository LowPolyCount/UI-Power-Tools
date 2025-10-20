// Copyright (c) Joel Gonzales

#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UIPowerTools.h"
#include "UI/Screens/Screen.h"

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

UScreenComponent* IUICSScreenAccessor::GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const
{
	UScreenComponent* RetVal = nullptr;
	UScreenComponentManager* ComponentManager = GetComponentManager();
	if (ComponentManager)
	{
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

void IUICSScreenAccessor::NativePreConstruct(bool bIsDesignTime)
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
	return RetVal;
}
