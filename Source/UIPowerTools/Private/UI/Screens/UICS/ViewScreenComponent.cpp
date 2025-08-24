// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UIPowerTools.h"
#include "UI/Screens/Screen.h"
#include "UObject/UObjectGlobals.h"
#include "UI/Screens/UICS/DataScreenComponent.h"
#include "UI/Screens/UICS/EntryScreenComponent.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"

void UViewScreenComponent::Initialize()
{
	Super::Initialize();


	OnSelectionChange.AddDynamic(this, &UViewScreenComponent::HandleOnSelectedChange);
	OnAction.AddDynamic(this, &UViewScreenComponent::HandleOnAction);
	OnFocusChange.AddDynamic(this, &UViewScreenComponent::HandleOnFocusChange);
	OnHoverChange.AddDynamic(this, &UViewScreenComponent::HandleOnHoverChange);

	SetLinkedDataComponent(GetScreenComponentFromSelector<UDataScreenComponent>(DataToListenTo));
}

void UViewScreenComponent::NativePreConstruct(bool bIsDesignTime)
{
	Super::NativePreConstruct(bIsDesignTime);
	if (PanelSelector.IsValid())
	{
		Panel = Cast<UPanelWidget>(PanelSelector.GetWidget(Cast<UWidget>(GetScreen())));

		// show preview of widgets in design view
		if (bIsDesignTime)
		{
			if (Panel)
			{
				Panel->ClearChildren();
				for (int32 i = 0; i < DesignEntriesToShow; ++i)
				{
					if (UUserWidget* EntryWidget = DuplicateObject<UUserWidget>(EntryWidgetPrototype, this))
					{
						Panel->AddChild(EntryWidget);
					}
				}
			}
		}
	}
}

void UViewScreenComponent::NativeDestruct()
{
	for(int32 i= ActiveViewWidgets.Num()-1; i >= 0; --i)
	{
		RemoveEntryWidget(ActiveViewWidgets[i]);
	}

	check(ActiveViewWidgets.Num() == 0);

	CachedWidgets.Empty();

	Super::NativeDestruct();

}

// Add default functionality here for any IUICSView functions that are not pure virtual.
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
	EntryWidgetPrototype = InWidgetPrototype;
	// flush system
	if (ActiveViewWidgets.Num() > 0)
	{
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

TScriptInterface<IEntryWidgetInterface> UViewScreenComponent::GetViewWidgetAt(int32 Index) const
{ 
	TScriptInterface<IEntryWidgetInterface> RetVal;

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

bool UViewScreenComponent::IsSelectedWidget() const
{
	return static_cast<bool>(GetFirstSelectedWidget());
}

TScriptInterface<IEntryWidgetInterface> UViewScreenComponent::GetFirstSelectedWidget() const
{
	TScriptInterface<IEntryWidgetInterface> RetVal;
	//@todo: Hold on to selected widgets so iteration is not required
	for (TScriptInterface<IEntryWidgetInterface> Widget : ActiveViewWidgets)
	{
		if (Widget->Execute_IsSelected(Widget.GetObject()))
		{
			RetVal = Widget;
			break;
		}
	}

	return RetVal;
}

TArray<TScriptInterface<IEntryWidgetInterface>> UViewScreenComponent::GetAllSelectedWidgets() const
{
	TArray<TScriptInterface<IEntryWidgetInterface>> RetVal;
	//@todo: Hold on to selected widgets so iteration is not required
	for (TScriptInterface<IEntryWidgetInterface> Widget : ActiveViewWidgets)
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

void UViewScreenComponent::HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries)
{
	// do we need to remove widgets to meet the new number of entries?
	const int32 WidgetDifference = ActiveViewWidgets.Num() - Entries.Num();
	if (WidgetDifference > 0)
	{
		for (int32 j = WidgetDifference, i = ActiveViewWidgets.Num() - 1; j > 0; --j, --i)
		{
			RemoveEntryWidget(ActiveViewWidgets[i]);
		}
	}
	else if (WidgetDifference < 0)
	{
		for (int32 i = WidgetDifference; i < 0; ++i)
		{
			GetAndSetupEntryWidget();
		}
	}

	for (int32 i = 0; i < Entries.Num(); ++i)
	{
		if(ActiveViewWidgets.IsValidIndex(i))
		{
			ActiveViewWidgets[i]->Execute_Reset(ActiveViewWidgets[i].GetObject());
			ActiveViewWidgets[i]->Execute_SetEntryData(ActiveViewWidgets[i].GetObject(), i, Entries[i]);

			if (Panel)
			{
				Panel->AddChild(Cast<UWidget>(ActiveViewWidgets[i].GetObject()));
			}
		}
		else
		{
			UE_LOG(LogUICS, Warning, TEXT(" There are %i Entries but only %i ActiveViewWidgets. There is no widget for Entry %i"), Entries.Num(), ActiveViewWidgets.Num(), i);
		}
	}
}

TScriptInterface<IEntryWidgetInterface> UViewScreenComponent::GetAndSetupEntryWidget()
{
	TScriptInterface<IEntryWidgetInterface> RetVal;
	if (CachedWidgets.Num() > 0 && bCacheWidgets)
	{
		RetVal = CachedWidgets[CachedWidgets.Num() - 1];
		CachedWidgets.RemoveAt(CachedWidgets.Num() - 1);
	}
	else if(EntryWidgetPrototype)
	{
		RetVal = DuplicateObject<UUserWidget>(EntryWidgetPrototype, this);
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No widget prototype specified. Cannot duplicate object for entry"));
	}

	if (RetVal)
	{
		ActiveViewWidgets.Emplace(RetVal);
		ListenToWidgetDelegates(RetVal);
	}

	AddToPanel(RetVal);


	return RetVal;
}

void UViewScreenComponent::AddToPanel(TScriptInterface<IEntryWidgetInterface>& Widget)
{
	if (Panel)
	{
		Panel->AddChild(Cast<UUserWidget>(Widget.GetObject()));
	}
}

void UViewScreenComponent::RemoveEntryWidget(TScriptInterface<IEntryWidgetInterface> Widget)
{
	if (Widget)
	{
		UUserWidget* AsUserWidget = Cast<UUserWidget>(Widget.GetObject());
		// @todo: Currently, we don't hold on to any swidgets for widgets that are no longer used.  

		Widget->Execute_Reset(Widget.GetObject());

		RemoveWidgetDelegates(Widget);
		if (bCacheWidgets)
		{
			CachedWidgets.Add(Widget);
		}

		ActiveViewWidgets.Remove(Widget);

		AsUserWidget->RemoveFromParent();
	}
}

void UViewScreenComponent::ListenToWidgetDelegates(TScriptInterface<IEntryWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->GetOnAction().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnHoverChange);
	}
}

void UViewScreenComponent::RemoveWidgetDelegates(TScriptInterface<IEntryWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->GetOnAction().RemoveDynamic(this, &UViewScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().RemoveDynamic(this, &UViewScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().RemoveDynamic(this, &UViewScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UViewScreenComponent::HandleWidgetOnHoverChange);
	}
}

void UViewScreenComponent::HandleWidgetOnAction(TScriptInterface<IEntryWidgetInterface> Widget)
{
	OnAction.Broadcast(this, Widget);
}
void UViewScreenComponent::HandleWidgetOnFocusChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained)
{
	OnFocusChange.Broadcast(this, Widget, bGained);
}

void UViewScreenComponent::HandleWidgetOnHoverChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained)
{
	OnHoverChange.Broadcast(this, Widget, bGained);
}

void UViewScreenComponent::HandleWidgetOnSelectionChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained)
{
	if (bSingleSelection && bGained)
	{
		for (TScriptInterface<IEntryWidgetInterface> ActiveWidget : ActiveViewWidgets)
		{
			if (ActiveWidget != Widget && ActiveWidget->Execute_IsSelected(ActiveWidget.GetObject()))
			{
				ActiveWidget->Execute_SetSelected(ActiveWidget.GetObject(), false);
			}
		}
	}
	OnSelectionChange.Broadcast(this, Widget, bGained);
}
