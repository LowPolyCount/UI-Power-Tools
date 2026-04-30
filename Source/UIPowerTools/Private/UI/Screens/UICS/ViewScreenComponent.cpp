// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UIPowerTools.h"
#include "UObject/UObjectGlobals.h"
#include "UI/Screens/UICS/DataScreenComponent.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/Utility/UIPTStatics.h"

FCachedWidget::FCachedWidget(const TScriptInterface<IViewWidgetInterface>& InWidget)
	: UserWidget(InWidget)
{ 
	if (UUserWidget* AsUserWidget = Cast<UUserWidget>(UserWidget.GetObject()))
	{
		SlateWidget = AsUserWidget->GetCachedWidget();
	}
}

void UViewScreenComponent::Initialize()
{
	Super::Initialize();

	OnSelectionChange.AddDynamic(this, &UViewScreenComponent::HandleOnSelectedChange);
	OnInputAction.AddDynamic(this, &UViewScreenComponent::HandleOnInputAction);
	OnFocusChange.AddDynamic(this, &UViewScreenComponent::HandleOnFocusChange);
	OnHoverChange.AddDynamic(this, &UViewScreenComponent::HandleOnHoverChange);

	SetLinkedDataComponent(UUIPTStatics::GetScreenComponentFromSelector<UDataScreenComponent>(this, DataToListenTo));
}


void UViewScreenComponent::NativePreConstruct(bool bIsDesignTime)
{
	Super::NativePreConstruct(bIsDesignTime);
	if (PanelSelector.IsValid())
	{
		Panel = Cast<UPanelWidget>(PanelSelector.GetWidget(Cast<UWidget>(UUIPTStatics::GetScreenAccessor(this).GetObject())));

		// show preview of widgets in design view
		if (bIsDesignTime)
		{
			SetupPreConstructWidgets();
		}
	}
}

void UViewScreenComponent::SetupPreConstructWidgets()
{
	if (Panel)
	{
		// PreConstruct can run multiple times while in design time. 
		// Don't clear the panel so we preserve any widgets the designer has added
		// Instead, Remove existing View Widgets and recreate them in case properties have changed. 
		for (TScriptInterface<IViewWidgetInterface> ViewWidget : ActiveViewWidgets)
		{
			if (UWidget* AsUWidget = Cast<UWidget>(ViewWidget.GetObject()))
			{
				if (Panel->HasChild(AsUWidget))
				{
					Panel->RemoveChild(AsUWidget);
				}
			}
		}

		ActiveViewWidgets.Empty();

		for (int32 i = 0; i < DesignEntriesToShow; ++i)
		{
			TScriptInterface<IViewWidgetInterface> ViewWidget = DuplicateWidget(ViewWidgetPrototype);
			AddToPanel(ViewWidget);
			ActiveViewWidgets.Emplace(ViewWidget);
		}
	}
}

// in it's own function to make sure we're consistent with how we duplicate
TScriptInterface<IViewWidgetInterface> UViewScreenComponent::DuplicateWidget(const TObjectPtr<UUserWidget>& Prototype)
{
	TScriptInterface<IViewWidgetInterface> RetVal = DuplicateObject<UUserWidget>(ViewWidgetPrototype, this);
	ensure(IsValid(RetVal.GetObject()));
	return RetVal;
}


void UViewScreenComponent::NativeDestruct()
{
	for(int32 i= ActiveViewWidgets.Num()-1; i >= 0; --i)
	{
		RemoveViewWidget(ActiveViewWidgets[i]);
	}

	//check(ActiveViewWidgets.Num() == 0);

	CachedWidgets.Empty();

	Super::NativeDestruct();

}

UWidget* UViewScreenComponent::GetDesiredFocusTarget() const
{
	UWidget* RetVal = nullptr;
	//ensureMsgf(InitialFocus, TEXT("bInitialFocus is false for %s"), *this->GetName());
	
	for (TScriptInterface<IViewWidgetInterface> ViewWidget : GetAllViewWidgets())
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

	return RetVal;
}

void UViewScreenComponent::SetLinkedDataComponent(UDataScreenComponent* InDataComponent)
{
	if (LinkedDataComponent)
	{
		LinkedDataComponent->OnDataRetrieval.RemoveDynamic(this, &UViewScreenComponent::HandleOnDataRetrieval);
	}

	LinkedDataComponent = InDataComponent;
	if (LinkedDataComponent)
	{
		LinkedDataComponent->OnDataRetrieval.AddDynamic(this, &UViewScreenComponent::HandleOnDataRetrieval);
	}
}

void UViewScreenComponent::SetWidgetPrototype(UUserWidget* InWidgetPrototype)
{
	ViewWidgetPrototype = InWidgetPrototype;
	// flush system
	if (ActiveViewWidgets.Num() > 0)
	{
		//@todo: We should handle this case and recreate the widgets from data. 
		UE_LOG(LogUICS, Warning, TEXT("There were %i Active Widgets when setting View Prototype. They will not be recreated, you will have to set the data for them again"), ActiveViewWidgets.Num());
	}
	ActiveViewWidgets.Empty();
	CachedWidgets.Empty();
}

void UViewScreenComponent::SetWidgetPrototypeByClass(UClass* WidgetPrototypeClass)
{
	if(this->GetWorld())
	{
		SetWidgetPrototype(UUserWidget::CreateWidgetInstance(*(this->GetWorld()), WidgetPrototypeClass, TEXT("WidgetPrototype")));
	}
	else
	{
		SetWidgetPrototype(NewObject<UUserWidget>(this, WidgetPrototypeClass));
	}
}

TScriptInterface<IViewWidgetInterface> UViewScreenComponent::GetViewWidgetAt(int32 Index) const
{ 
	TScriptInterface<IViewWidgetInterface> RetVal;

	if(ActiveViewWidgets.IsValidIndex(Index))
	{
		RetVal = ActiveViewWidgets[Index];
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No Widget in Index %i"), Index);
	}

	return RetVal;
}

TArray<UUserWidget*> UViewScreenComponent::GetAllWidgets() const
{
	TArray<UUserWidget*> RetVal;
	for (const TScriptInterface<IViewWidgetInterface>& AsViewWidget : ActiveViewWidgets)
	{
		if (UUserWidget* AsUserWidget = Cast<UUserWidget>(AsViewWidget.GetObject()))
		{
			RetVal.Emplace(AsUserWidget);
		}
	}

	return RetVal;
}

UUserWidget* UViewScreenComponent::GetWidgetAt(int32 Index) const
{
	UUserWidget* RetVal = nullptr;

	TScriptInterface<IViewWidgetInterface> ViewWidget = GetViewWidgetAt(Index);

	if (UUserWidget* AsUserWidget = Cast<UUserWidget>(ViewWidget.GetObject()))
	{
		RetVal = AsUserWidget;
	}

	return RetVal;
}

bool UViewScreenComponent::IsSelectedWidget() const
{
	return static_cast<bool>(GetFirstSelectedWidget());
}

TScriptInterface<IViewWidgetInterface> UViewScreenComponent::GetFirstSelectedWidget() const
{
	TScriptInterface<IViewWidgetInterface> RetVal;
	//@todo: Hold on to selected widgets so iteration is not required
	for (TScriptInterface<IViewWidgetInterface> Widget : ActiveViewWidgets)
	{
		if (Widget->Execute_IsSelected(Widget.GetObject()))
		{
			RetVal = Widget;
			break;
		}
	}

	return RetVal;
}

TArray<TScriptInterface<IViewWidgetInterface>> UViewScreenComponent::GetAllSelectedWidgets() const
{
	TArray<TScriptInterface<IViewWidgetInterface>> RetVal;
	//@todo: Hold on to selected widgets so iteration is not required
	for (TScriptInterface<IViewWidgetInterface> Widget : ActiveViewWidgets)
	{
		if (Widget->Execute_IsSelected(Widget.GetObject()))
		{
			RetVal.Emplace(Widget);
		}
	}
	return RetVal;
}

void UViewScreenComponent::ManuallySetData(const TArray<UObject*>& Entries)
{
	HandleOnDataRetrieval(nullptr, Entries);
}

void UViewScreenComponent::SetLinkedActionScreenComponent(UActionScreenComponent* InASC)
{
	LinkedASC = InASC;
}

void UViewScreenComponent::HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries)
{
	PopulateWidgets(Entries);

}

void UViewScreenComponent::PopulateWidgets(const TArray<UObject*>& Entries)
{
	// do we need to remove widgets to meet the new number of entries?
	const int32 WidgetDifference = ActiveViewWidgets.Num() - Entries.Num();

	// we know how many entries we need upfront, so reserve them.
	ActiveViewWidgets.Reserve(Entries.Num());
	CachedWidgets.Reserve(CachedWidgets.Num() + WidgetDifference);

	// let's remove any unused widgets
	if (WidgetDifference > 0)
	{
		for (int32 j = WidgetDifference, i = ActiveViewWidgets.Num() - 1; j > 0; --j, --i)
		{
			RemoveViewWidget(ActiveViewWidgets[i]);
		}
	}
	else if (WidgetDifference < 0)	// let's create any widgets that we will need
	{
		for (int32 i = WidgetDifference; i < 0; ++i)
		{
			GetAndSetupEntryWidget();
		}
	}

	// update data for our widgets
	for (int32 i = 0; i < Entries.Num(); ++i)
	{
		if (ActiveViewWidgets.IsValidIndex(i))
		{
			if (Panel)
			{
				AddToPanel(ActiveViewWidgets[i]);
			}

			ActiveViewWidgets[i]->Execute_Reset(ActiveViewWidgets[i].GetObject());
			ActiveViewWidgets[i]->Execute_SetEntryData(ActiveViewWidgets[i].GetObject(), i, Entries[i]);
		}
		else
		{
			UE_LOG(LogUICS, Warning, TEXT(" There are %i Entries but only %i ActiveViewWidgets. There is no widget for Entry %i"), Entries.Num(), ActiveViewWidgets.Num(), i);
		}
	}

	if (OnWidgetsPopulated.IsBound())
	{
		OnWidgetsPopulated.Broadcast(this);
	}


	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_WidgetsPopulated))
	{
		struct {
			UViewScreenComponent* Component;
		} Args = { this };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

TScriptInterface<IViewWidgetInterface> UViewScreenComponent::GetAndSetupEntryWidget()
{
	TScriptInterface<IViewWidgetInterface> RetVal;
	int32 FoundIndex = INDEX_NONE;

	if (CachedWidgets.Num() > 0 && bCacheWidgets)
	{
		FoundIndex = CachedWidgets.Num() - 1;
		RetVal = CachedWidgets[FoundIndex].UserWidget;
		
	}
	else if(ViewWidgetPrototype)
	{
		RetVal = DuplicateWidget(ViewWidgetPrototype);
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No widget prototype specified. Cannot duplicate object for entry"));
	}

	if (RetVal)
	{
		ActiveViewWidgets.Emplace(RetVal);
		ViewWidgetSetup(RetVal);
		AddToPanel(RetVal);

		// we can't remove the entry from CachedWidgets until after AddToPanel() otherwise the slate widget will be destroyed
		if (FoundIndex != INDEX_NONE)
		{
			CachedWidgets.RemoveAt(FoundIndex);
		}
	}

	return RetVal;
}

void UViewScreenComponent::AddToPanel(TScriptInterface<IViewWidgetInterface>& Widget)
{
	if (Panel)
	{
		Panel->AddChild(Cast<UUserWidget>(Widget.GetObject()));
	}
}

void UViewScreenComponent::RemoveViewWidget(TScriptInterface<IViewWidgetInterface> Widget)
{
	if (Widget)
	{
		if (UUserWidget* AsUserWidget = Cast<UUserWidget>(Widget.GetObject()))
		{
			Widget->Execute_Reset(Widget.GetObject());

			ViewWidgetTeardown(Widget);

			
			if (bCacheWidgets)
			{
				// must add the widget to CachedWidgets Before RemoveFromParent() is called, otherwise the SWidget will be destroyed
				CachedWidgets.Add(FCachedWidget(Widget));
			}

			ActiveViewWidgets.Remove(Widget);

			AsUserWidget->RemoveFromParent();
		}
	}
}

void UViewScreenComponent::ViewWidgetSetup(TScriptInterface<IViewWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->SetOwningViewScreenComponent(this);
		Widget->GetOnAction().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnHoverChange);
	}
}

void UViewScreenComponent::ViewWidgetTeardown(TScriptInterface<IViewWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->SetOwningViewScreenComponent(nullptr);
		Widget->GetOnAction().RemoveDynamic(this, &UViewScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().RemoveDynamic(this, &UViewScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().RemoveDynamic(this, &UViewScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnHoverChange);
	}
}

void UViewScreenComponent::HandleWidgetOnAction(TScriptInterface<IViewWidgetInterface> Widget)
{
	OnAction.Broadcast(this, Widget);
	OnInputAction.Broadcast(this, Widget);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_InputAction))
	{
		struct {
			UViewScreenComponent* Component;
			const TScriptInterface<IViewWidgetInterface>& Widget;
		} Args = { this, Widget };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UViewScreenComponent::HandleWidgetOnFocusChange(TScriptInterface<IViewWidgetInterface> Widget, bool bGained)
{
	OnFocusChange.Broadcast(this, Widget, bGained);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_FocusChange))
	{
		struct {
			UViewScreenComponent* Component;
			const TScriptInterface<IViewWidgetInterface>& Widget;
			bool bGained;
		} Args = { this, Widget, bGained };

		ProcessFuncFromResolveMember(Func, &Args);
	}	
}

void UViewScreenComponent::HandleWidgetOnHoverChange(TScriptInterface<IViewWidgetInterface> Widget, bool bGained)
{
	OnHoverChange.Broadcast(this, Widget, bGained);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_HoverChange))
	{
		struct {
			UViewScreenComponent* Component;
			const TScriptInterface<IViewWidgetInterface>& Widget;
			bool bGained;
		} Args = { this, Widget, bGained };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UViewScreenComponent::HandleWidgetOnSelectionChange(TScriptInterface<IViewWidgetInterface> Widget, bool bGained)
{
	if (bSingleSelection && bGained)
	{
		for (TScriptInterface<IViewWidgetInterface> ActiveWidget : ActiveViewWidgets)
		{
			if (ActiveWidget != Widget && ActiveWidget->Execute_IsSelected(ActiveWidget.GetObject()))
			{
				ActiveWidget->Execute_SetSelected(ActiveWidget.GetObject(), false);
			}
		}
	}

	OnSelectionChange.Broadcast(this, Widget, bGained);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_SelectionChange))
	{
		struct {
			UViewScreenComponent* Component;
			const TScriptInterface<IViewWidgetInterface>& Widget;
			bool bGained;
		} Args = { this, Widget, bGained };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}