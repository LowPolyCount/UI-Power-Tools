// Copyright 2025 Joel Gonzales


#include "UI/Utility/UIPTStatics.h"
#include <Components/PanelWidget.h>
#include <Blueprint/WidgetTree.h>
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "Components/Widget.h"
#include "UI/Screens/UICS/IUICSAccessor.h"


UScreenComponent* UUIPTStatics::GetScreenComponent_BP(const UObject* PathToScreen, TSubclassOf<UScreenComponent> Type)
{
	UScreenComponent* RetVal = nullptr;

	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetScreenComponent_BP(Type);
	}
	return RetVal;
}

TArray<UScreenComponent*> UUIPTStatics::GetAllScreenComponents_BP(const UObject* PathToScreen, TSubclassOf<UScreenComponent> Type)
{
	TArray<UScreenComponent*> RetVal;
	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetAllScreenComponents_BP(Type);
	}
	return RetVal;
}

UScreenComponent* UUIPTStatics::GetScreenComponentByName_BP(const UObject* PathToScreen, const FName Name, TSubclassOf<UScreenComponent> Type)
{
	UScreenComponent* RetVal = nullptr;
	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetScreenComponentByName_BP(Name, Type);
	}
	return RetVal;
}

UScreenComponent* UUIPTStatics::GetScreenComponentFromSelector_BP(const UObject* PathToScreen, const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type)
{
	UScreenComponent* RetVal = nullptr;
	if (TScriptInterface<const IUICSScreenAccessor> Screen = GetScreenAccessor(PathToScreen))
	{
		RetVal = Screen->GetScreenComponentFromSelector_BP(Selector, Type);
	}
	return RetVal;
}

TScriptInterface<const IUICSScreenAccessor> UUIPTStatics::GetScreenAccessor(const UObject* PathToScreen)
{
	TScriptInterface<const IUICSScreenAccessor> RetVal = nullptr;

	// if we're a widget, it's more complicated to get to the screen because of WidgetTrees, etc. so GetImplementingOuterObject may not find the screen. 
	if (const UWidget* PathAsWidget = Cast<UWidget>(PathToScreen))
	{
		// check if the given PathToScreen is the Screen
		if (PathAsWidget->Implements<UUICSAccessor>())
		{
			// while we should set RetVal here and then break, for this code, it's easier to return out instead. 
			return PathAsWidget;
		}

		// this is a modified version of CommonUILibrary::FindParentWidgetOfType()
		const UWidget* StartingWidget = PathAsWidget;
		while (StartingWidget)
		{
			const UWidget* LocalRoot = StartingWidget;
			const UWidget* LocalParent = LocalRoot->GetParent();
			while (LocalParent)
			{
				if (LocalParent->Implements<UUICSAccessor>())
				{
					// while we should set RetVal and break, it's easier to return out here instead. 
					return LocalParent;
				}
				LocalRoot = LocalParent;
				LocalParent = LocalParent->GetParent();
			}

			UWidgetTree* WidgetTree = Cast<UWidgetTree>(LocalRoot->GetOuter());
			if (WidgetTree == nullptr)
			{
				break;
			}

			StartingWidget = Cast<UUserWidget>(WidgetTree->GetOuter());
			if (StartingWidget && StartingWidget->Implements<UUICSAccessor>())
			{
				return StartingWidget;
			}
		}
	}
	else // we're some other type of object (Like Screen Component) where can use GetImplementingOuterObject
	{
		if (PathToScreen)
		{
			if (const IUICSScreenAccessor* AsScreen = Cast<IUICSScreenAccessor>(PathToScreen))
			{
				RetVal = PathToScreen;
			}
			else
			{
				UObjectBaseUtility* Outer = PathToScreen->GetImplementingOuterObject(UUICSScreenAccessor::StaticClass());
				RetVal = TScriptInterface<const IUICSScreenAccessor>(Cast<UObject>(Outer));
			}
		}
	}

	// don't log or error if RetVal is null. Can happen in editor
	return RetVal;
}

TScriptInterface<IUICSScreenAccessor> UUIPTStatics::GetScreenAccessor(UObject* PathToScreen)
{
	// We're using const-cast so that we can reuse the code from the const version of this function

	// add const to the object so we call the const version of this function is called
	TScriptInterface<const IUICSScreenAccessor> FoundScreen = GetScreenAccessor(const_cast<const UObject*>(PathToScreen));

	// remove const as the passed in parameter PathToScreen is non-const. 
	return TScriptInterface<IUICSScreenAccessor>((const_cast<UObject*>(FoundScreen.GetObject())));
}


UWidget* UUIPTStatics::GetDesiredFocusTargetFromViewComponents(const UObject* PathToScreen)
{
	UWidget* RetVal = nullptr;
	for (const UViewScreenComponent* ViewComponent : UUIPTStatics::GetAllScreenComponents<UViewScreenComponent>(PathToScreen))
	{
		if (IsValid(ViewComponent) && ViewComponent->IsDesiredFocusTarget())
		{
			RetVal = ViewComponent->GetDesiredFocusTarget();
			if (IsValid(RetVal))
			{
				break;
			}
		}
	}
	return RetVal;
}
