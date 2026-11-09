// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information


#include "UICS/Screens/Components/Display/DisplayScreenComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UObjectGlobals.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/ListView.h"
#include "CommonButtonBase.h"
#include "UICSModule.h"
#include "UICS/Screens/Components/Data/DataScreenComponent.h"
#include "UICS/Screens/Components/Display/DisplayCommonButtonBase.h"
#include "Templates/SharedPointer.h"
#include "UICS/Utility/UIPTStatics.h"

FCachedWidget::FCachedWidget(const TScriptInterface<IDisplayWidgetInterface>& InWidget)
	: UserWidget(InWidget)
{ 
	if (UUserWidget* AsUserWidget = Cast<UUserWidget>(UserWidget.GetObject()))
	{
		SlateWidget = AsUserWidget->GetCachedWidget();
	}
}

void UDisplayScreenComponent::NativeInitialize()
{
	Super::NativeInitialize();

	OnSelectionChange.AddDynamic(this, &UDisplayScreenComponent::HandleOnSelectedChange);
	OnInputAction.AddDynamic(this, &UDisplayScreenComponent::HandleOnInputAction);
	OnFocusChange.AddDynamic(this, &UDisplayScreenComponent::HandleOnFocusChange);
	OnHoverChange.AddDynamic(this, &UDisplayScreenComponent::HandleOnHoverChange);

	SetLinkedDataComponent(UUIPTStatics::GetScreenComponentFromSelector<UDataScreenComponent>(this, DataToListenTo));

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Get))
	{
		struct {
			UDisplayScreenComponent* DisplayComponent;
		} Args = { this };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UDisplayScreenComponent::NativePreConstruct(bool bIsDesignTime)
{
	Super::NativePreConstruct(bIsDesignTime);
	if (PanelSelector.IsValid())
	{
		Panel = Cast<UWidget>(PanelSelector.GetSelectedWidget(Cast<UWidget>(UUIPTStatics::GetScreenAccessor(this).GetObject())));
		DeterminePanelCharacteristics(nullptr);
		if(IsDesignTime())
		{
			// show preview of widgets in design view
			SetupPreConstructWidgets();
		}
	}
}

void UDisplayScreenComponent::SetupPreConstructWidgets()
{
	if (Panel)
	{
		// PreConstruct can run multiple times while in design time. 
		// Don't clear the panel so we preserve any widgets the designer has added
		// Instead, Remove existing View Widgets and recreate them in case properties have changed. 
		for (TScriptInterface<IDisplayWidgetInterface> Displayidget : ActiveDisplayWidgets)
		{
			if (UWidget* AsUWidget = Cast<UWidget>(Displayidget.GetObject()))
			{
				if (UPanelWidget* AsPanel = Cast<UPanelWidget>(Panel))
				{
					if (AsPanel->HasChild(AsUWidget))
					{
						AsPanel->RemoveChild(AsUWidget);
					}
				}
			}
		}

		ActiveDisplayWidgets.Empty();

		for (int32 i = 0; i < DesignEntriesToShow; ++i)
		{
			if (TScriptInterface<IDisplayWidgetInterface> Displayidget = DuplicateWidget(DisplayWidgetPrototype))
			{
				AddToPanel(Displayidget);
				ActiveDisplayWidgets.Emplace(Displayidget);
			}
		}
	}
}

// in it's own function to make sure we're consistent with how we duplicate
TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::DuplicateWidget(const TObjectPtr<UUserWidget>& Prototype)
{
	TScriptInterface<IDisplayWidgetInterface> RetVal = DuplicateObject<UUserWidget>(DisplayWidgetPrototype, this);
	// we could ensure(RetVal) here, but PreConstruct() can call this func and that can happen during teardown. 
	return RetVal;
}


void UDisplayScreenComponent::NativeDestruct()
{
	if (bIsPanelListView)
	{
		TeardownPanelListViewListeners();
	}

	for(int32 i= ActiveDisplayWidgets.Num()-1; i >= 0; --i)
	{
		RemoveDisplayWidget(ActiveDisplayWidgets[i]);
	}
	ActiveDisplayWidgets.Empty();

	for (FCachedWidget& Widget : CachedWidgets)
	{
		Widget.UserWidget->Release();
	}
	CachedWidgets.Empty();
	//check(ActiveDisplayWidgets.Num() == 0);

	FocusedWidget = nullptr;

	Super::NativeDestruct();
}

#if WITH_EDITOR

void UDisplayScreenComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	for (FProperty* Property : PropertyChangedEvent.PropertyChain)
	{
		// @note: This will be hit multiple times when editing the PanelSelector, one time for each property that gets saved. 
		if (Property->GetName() == GET_MEMBER_NAME_CHECKED(FWidgetSelector, WidgetName))
		{
			DeterminePanelCharacteristicsForEditorSettings();
		}
	}
}

void UDisplayScreenComponent::DeterminePanelCharacteristicsForEditorSettings()
{
	if (UWidget* AsWidget = Cast<UWidget>(UUIPTStatics::GetScreenAccessor(this).GetObject()))
	{
		if (const UWidget* SelectedPanelWidget = PanelSelector.GetWidgetAtEditorTime(AsWidget))
		{
			const bool bPreviousIsAGrid = bIsPanelGrid;
			const bool bPreviousIsListView = bIsPanelListView;
			bIsPanelGrid = SelectedPanelWidget->GetClass() == UUniformGridPanel::StaticClass();
			bIsPanelListView = SelectedPanelWidget->GetClass() == UListView::StaticClass();

			if (bPreviousIsListView != bIsPanelListView)
			{
				// switching from a list view
				if (bPreviousIsListView)
				{
					bCacheWidgets = true;
					DisplayWidgetPrototype = nullptr;
					DesignEntriesToShow = 3;
				}
				else // switching to a list view
				{
					bCacheWidgets = false;
					DesignEntriesToShow = 0;
					bSingleSelection = false;
				}
			}
		}

	}
}

#endif

void UDisplayScreenComponent::DeterminePanelCharacteristics(UWidget* PreviousPanel)
{
	if (PreviousPanel && bIsPanelListView)
	{
		TeardownPanelListViewListeners();
	}

	if (Panel)
	{
		bIsPanelGrid = (Cast<UUniformGridPanel>(Panel) != nullptr);
		bIsPanelListView = (Cast<UListView>(Panel) != nullptr);

		if (!IsDesignTime() && bIsPanelListView)
		{
			// DisplayWidgetPrototype will be null if using a ListView. Use this as a stand-in so that we
			// create it as a prototype and can still use all our regular codepaths
			DisplayWidgetPrototype = NewObject<UDisplayWidgetListViewStandIn>();
			SetupPanelListViewListeners();
		}
	}
}

void UDisplayScreenComponent::SetPanel(UWidget* InPanel) 
{ 
	if (Panel != InPanel) 
	{
		UWidget* PreviousPanel = Panel;
		Panel = InPanel; 
		DeterminePanelCharacteristics(PreviousPanel);
	} 
}



UWidget* UDisplayScreenComponent::GetDesiredFocusTarget() const
{
	UWidget* RetVal = nullptr;
	//ensureMsgf(InitialFocus, TEXT("bInitialFocus is false for %s"), *this->GetName());
	
	for (TScriptInterface<IDisplayWidgetInterface> Displayidget : GetAllDisplayWidgets())
	{
		if (UUserWidget* AsUWidget = Cast<UUserWidget>(Displayidget.GetObject()))
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

void UDisplayScreenComponent::SetLinkedDataComponent(UDataScreenComponent* InDataComponent)
{
	if (LinkedDataComponent)
	{
		LinkedDataComponent->OnDataRetrieval.RemoveDynamic(this, &UDisplayScreenComponent::HandleOnDataRetrieval);
	}

	LinkedDataComponent = InDataComponent;
	if (LinkedDataComponent)
	{
		LinkedDataComponent->OnDataRetrieval.AddDynamic(this, &UDisplayScreenComponent::HandleOnDataRetrieval);
	}
}

void UDisplayScreenComponent::SetWidgetPrototype(UUserWidget* InWidgetPrototype)
{
	DisplayWidgetPrototype = InWidgetPrototype;
	// flush system
	if (ActiveDisplayWidgets.Num() > 0)
	{
		//@todo: We should handle this case and recreate the widgets from data. 
		UE_LOG(LogUICS, Warning, TEXT("There were %i Active Widgets when setting Display Prototype. They will not be recreated, you will have to set the data for them again"), ActiveDisplayWidgets.Num());
	}
	ActiveDisplayWidgets.Empty();
	CachedWidgets.Empty();
}

void UDisplayScreenComponent::SetWidgetPrototypeByClass(UClass* WidgetPrototypeClass)
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

TArray<UUserWidget*> UDisplayScreenComponent::GetAllDisplayWidgetsAsUserWidgets() const
{
	TArray<UUserWidget*> RetVal;
	for (const TScriptInterface<IDisplayWidgetInterface>& AsViewWidget : ActiveDisplayWidgets)
	{
		if (UUserWidget* AsUserWidget = Cast<UUserWidget>(AsViewWidget.GetObject()))
		{
			RetVal.Emplace(AsUserWidget);
		}
	}

	return RetVal;
}

UUserWidget* UDisplayScreenComponent::GetWidgetAt(int32 Index) const
{
	UUserWidget* RetVal = nullptr;

	TScriptInterface<IDisplayWidgetInterface> ViewWidget = GetDisplayWidgetAt(Index);
	if (UUserWidget* AsUserWidget = Cast<UUserWidget>(ViewWidget.GetObject()))
	{
		RetVal = AsUserWidget;
	}

	return RetVal;
}

TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::GetDisplayWidgetUsingEntryData(const UObject* EntryData) const
{
	TScriptInterface<IDisplayWidgetInterface> RetVal;

	if (bIsPanelListView)
	{
		if (UListView* AsListView = Cast<UListView>(Panel))
		{
			RetVal = AsListView->GetEntryWidgetFromItem(EntryData);
		}
	}
	else // normal panel
	{
		for (TScriptInterface<IDisplayWidgetInterface> Widget : ActiveDisplayWidgets)
		{
			if (Widget->Execute_GetEntryData(Widget.GetObject()) == EntryData)
			{
				RetVal = Widget;
				break;
			}
		}
	}

	return RetVal;
}

TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::GetDisplayWidgetAt(int32 Index) const
{
	TScriptInterface<IDisplayWidgetInterface> RetVal;

	if (ActiveDisplayWidgets.IsValidIndex(Index))
	{
		RetVal = ActiveDisplayWidgets[Index];
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No Widget in Index %i"), Index);
	}

	return RetVal;
}

bool UDisplayScreenComponent::IsFocused() const 
{ 
	return IsValid(FocusedWidget.GetObject()); 
}

TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::GetFocusedWidget() const 
{ 
	return FocusedWidget; 
}

bool UDisplayScreenComponent::IsAWidgetSelected() const
{
	return static_cast<bool>(GetFirstSelectedWidget());
}

bool UDisplayScreenComponent::IsSelectedWidget() const
{
	return IsAWidgetSelected();
}

TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::GetFirstSelectedWidget() const
{
	TScriptInterface<IDisplayWidgetInterface> RetVal;
	//@todo: Hold on to selected widgets so iteration is not required
	for (TScriptInterface<IDisplayWidgetInterface> Widget : ActiveDisplayWidgets)
	{
		if (const UCommonButtonBase* AsButton = Cast<UCommonButtonBase>(Widget.GetObject()))
		{
			if (AsButton->GetSelected())
			{
				RetVal = Widget;
				break;
			}
		}
	}

	return RetVal;
}

TArray<TScriptInterface<IDisplayWidgetInterface>> UDisplayScreenComponent::GetAllSelectedWidgets() const
{
	TArray<TScriptInterface<IDisplayWidgetInterface>> RetVal;
	//@todo: Hold on to selected widgets so iteration is not required
	
	if (bIsPanelListView)
	{
		if (UListView* AsListView = Cast<UListView>(Panel))
		{
			TArray<UObject*> SelectedEntries;
			AsListView->GetSelectedItems(SelectedEntries);
			for (UObject* Entry : SelectedEntries)
			{
				if (UUserWidget* UserWidget = AsListView->GetEntryWidgetFromItem(Entry))
				{
					RetVal.Add(TScriptInterface<IDisplayWidgetInterface>(UserWidget));
				}
			}
		}
	}
	else
	{
		for (TScriptInterface<IDisplayWidgetInterface> Widget : ActiveDisplayWidgets)
		{
			if (const UCommonButtonBase* AsButton = Cast<UCommonButtonBase>(Widget.GetObject()))
			{
				if (AsButton->GetSelected())
				{
					RetVal.Emplace(Widget);
				}
			}
		}
	}

	return RetVal;
}

void UDisplayScreenComponent::ManuallySetData(const TArray<UObject*>& Entries)
{
	HandleOnDataRetrieval(nullptr, Entries);
}

void UDisplayScreenComponent::SetLinkedActionScreenComponent(UActionScreenComponent* InASC)
{
	LinkedASC = InASC;
}

void UDisplayScreenComponent::HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries)
{
	PopulateWidgets(Entries);

}

void UDisplayScreenComponent::PopulateWidgets(const TArray<UObject*>& Entries)
{
	// do we need to remove widgets to meet the new number of entries?
	const int32 WidgetDifference = ActiveDisplayWidgets.Num() - Entries.Num();

	// we know how many entries we need upfront, so reserve them.
	ActiveDisplayWidgets.Reserve(Entries.Num());

	if ((CachedWidgets.Num() + WidgetDifference) > 0)
	{
		CachedWidgets.Reserve(CachedWidgets.Num() + WidgetDifference);
	}

	

	// let's remove any unused widgets
	if (WidgetDifference > 0)
	{
		for (int32 j = WidgetDifference, i = ActiveDisplayWidgets.Num() - 1; j > 0; --j, --i)
		{
			RemoveDisplayWidget(ActiveDisplayWidgets[i]);
		}
	}
	else if (WidgetDifference < 0)	// let's create any widgets that we will need
	{
		for (int32 i = WidgetDifference; i < 0; ++i)
		{
			GetAndSetupEntryWidget();
		}
	}

	// With Normal Panels, Our widgets we expect to be added to the panel before calling SetEntryData.
	// For ListViews, It needs to be in the opposite order because ListViews take in data from a DisplayWidget
	// this is a cludge for now, would like to refactor this into some type of adaptor class. 
	if (bIsPanelListView)
	{
		// list views do a bunch of things on their own that we can't override, so handle that and exit out. 
		if (UListView* AsListView = Cast<UListView>(Panel))
		{
			AsListView->ClearListItems();
			for (int32 i = 0; i < Entries.Num(); ++i)
			{
				ActiveDisplayWidgets[i]->Execute_SetEntryData(ActiveDisplayWidgets[i].GetObject(), i, Entries[i]);
				AddToPanel(ActiveDisplayWidgets[i]);
			}
		}
	}
	else
	{
		// update data for our widgets
		for (int32 i = 0; i < Entries.Num(); ++i)
		{
			if (ActiveDisplayWidgets.IsValidIndex(i))
			{
				ActiveDisplayWidgets[i]->Execute_Reset(ActiveDisplayWidgets[i].GetObject());
				ActiveDisplayWidgets[i]->Execute_SetEntryData(ActiveDisplayWidgets[i].GetObject(), i, Entries[i]);
			}
			else
			{
				UE_LOG(LogUICS, Warning, TEXT(" There are %i Entries but only %i ActiveDisplayWidgets. There is no widget for Entry %i"), Entries.Num(), ActiveDisplayWidgets.Num(), i);
			}
		}
	}

	NotifyThatWidgetsArePopulated();

	// if a widget is focused, then it's data could have been changed.  Call HandleOnFocusChange to simulate it gaining focus so that 
	// tooltips and other listeners to display focus events will get the new data. 
	if (IsFocused())
	{
		HandleOnFocusChange(this, FocusedWidget, true);
	}
}

TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::GetAndSetupEntryWidget()
{
	TScriptInterface<IDisplayWidgetInterface> RetVal;
	int32 FoundCachedWidgetIndex = INDEX_NONE;	// if we use a cached widget, this was it's index in the array

	if (CachedWidgets.Num() > 0 && bCacheWidgets)
	{
		FoundCachedWidgetIndex = CachedWidgets.Num() - 1;
		RetVal = CachedWidgets[FoundCachedWidgetIndex].UserWidget;
		
	}
	else if(DisplayWidgetPrototype)
	{
		RetVal = DuplicateWidget(DisplayWidgetPrototype);
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No widget prototype specified. Cannot duplicate object for entry"));
	}

	if (RetVal)
	{
		ActiveDisplayWidgets.Emplace(RetVal);
		DisplayWidgetSetup(RetVal);
		AddToPanel(RetVal);

		// we can't remove the entry from CachedWidgets until after AddToPanel() otherwise the slate widget will be destroyed
		if (FoundCachedWidgetIndex != INDEX_NONE)
		{
			CachedWidgets.RemoveAt(FoundCachedWidgetIndex);
		}
	}

	return RetVal;
}

void UDisplayScreenComponent::NotifyThatWidgetsArePopulated()
{
	if (OnWidgetsPopulated.IsBound())
	{
		OnWidgetsPopulated.Broadcast(this);
	}

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_WidgetsPopulated))
	{
		struct {
			UDisplayScreenComponent* Component;
		} Args = { this };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UDisplayScreenComponent::AddToPanel(TScriptInterface<IDisplayWidgetInterface>& Widget)
{
	if (Panel && Widget)
	{
		if (bIsPanelGrid)
		{
			if (UUniformGridPanel* AsGrid = Cast<UUniformGridPanel>(Panel))
			{
				// the widget has already been added to ActiveDisplayWidgets, so we need to call Panel->GetChildrenCount() to get the correct number
				const int32 NumChildren = AsGrid->GetChildrenCount();
				const int32 ColumnX = NumChildren % ColumnsGridWillHave;
				const int32 RowY = (ColumnsGridWillHave == 0) ? NumChildren : (NumChildren / ColumnsGridWillHave);

				AsGrid->AddChildToUniformGrid(Cast<UWidget>(Widget.GetObject()), RowY, ColumnX);
			}
			else
			{
				//@todo: Log
			}
		}
		else if (bIsPanelListView)
		{
			if (UListView* AsListView = Cast<UListView>(Panel))
			{
				AsListView->AddItem(Widget->Execute_GetEntryData(Widget.GetObject()));
			}
			else
			{
				//@todo: Log
			}
		}
		else if (UPanelWidget* AsPanel = Cast<UPanelWidget>(Panel))
		{
			AsPanel->AddChild(Cast<UUserWidget>(Widget.GetObject()));
		}
		else
		{
			//@todo: Log
		}
	}
}

void UDisplayScreenComponent::RemoveDisplayWidget(TScriptInterface<IDisplayWidgetInterface> Widget)
{
	if (Widget)
	{
		if (bCacheWidgets)
		{
			// must add the widget to CachedWidgets Before RemoveFromParent() is called, otherwise the SWidget will be destroyed
			CachedWidgets.Add(FCachedWidget(Widget));
		}

		Widget->Execute_Reset(Widget.GetObject());
		DisplayWidgetTeardown(Widget);
		ActiveDisplayWidgets.Remove(Widget);


		if (Panel && bIsPanelListView)
		{
			// we have to remove from listviews using it's removeItem function. 
			if (UListView* AsListView = Cast<UListView>(Panel))
			{
				AsListView->RemoveItem(Widget->Execute_GetEntryData(Widget.GetObject()));
			}
			else
			{
				//@todo: Log
			}
		}
		// for normal removals, call RemoveFromParent in-case the program has changed the parent. 
		else if (UWidget* AsUserWidget = Cast<UWidget>(Widget.GetObject()))
		{
			AsUserWidget->RemoveFromParent();
		}
		else
		{
			//@todo: Log
		}
	}
}

void UDisplayScreenComponent::DisplayWidgetSetup(TScriptInterface<IDisplayWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->SetOwningDisplayScreenComponent(this);
		Widget->GetOnAction().AddUniqueDynamic(this, &UDisplayScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().AddUniqueDynamic(this, &UDisplayScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().AddUniqueDynamic(this, &UDisplayScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UDisplayScreenComponent::HandleWidgetOnHoverChange);
	}
}

void UDisplayScreenComponent::DisplayWidgetTeardown(TScriptInterface<IDisplayWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->Release();
		Widget->GetOnAction().RemoveDynamic(this, &UDisplayScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().RemoveDynamic(this, &UDisplayScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().RemoveDynamic(this, &UDisplayScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UDisplayScreenComponent::HandleWidgetOnHoverChange);
		Widget->SetOwningDisplayScreenComponent(nullptr);
	}
}

void UDisplayScreenComponent::HandleWidgetOnAction(TScriptInterface<IDisplayWidgetInterface> Widget)
{
	OnAction.Broadcast(this, Widget);
	OnInputAction.Broadcast(this, Widget);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_InputAction))
	{
		struct {
			UDisplayScreenComponent* Component;
			const TScriptInterface<IDisplayWidgetInterface>& Widget;
		} Args = { this, Widget };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UDisplayScreenComponent::HandleWidgetOnFocusChange(TScriptInterface<IDisplayWidgetInterface> Widget, bool bGained)
{
	if (bGained)
	{
		FocusedWidget = Widget;
	}
	else
	{
		FocusedWidget = nullptr;
	}
	
	OnFocusChange.Broadcast(this, Widget, bGained);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_FocusChange))
	{
		struct {
			UDisplayScreenComponent* Component;
			const TScriptInterface<IDisplayWidgetInterface>& Widget;
			bool bGained;
		} Args = { this, Widget, bGained };

		ProcessFuncFromResolveMember(Func, &Args);
	}	
}

void UDisplayScreenComponent::HandleWidgetOnHoverChange(TScriptInterface<IDisplayWidgetInterface> Widget, bool bGained)
{
	OnHoverChange.Broadcast(this, Widget, bGained);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_HoverChange))
	{
		struct {
			UDisplayScreenComponent* Component;
			const TScriptInterface<IDisplayWidgetInterface>& Widget;
			bool bGained;
		} Args = { this, Widget, bGained };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UDisplayScreenComponent::HandleWidgetOnSelectionChange(TScriptInterface<IDisplayWidgetInterface> Widget, bool bGained)
{
	if (bSingleSelection && bGained)
	{
		for (TScriptInterface<IDisplayWidgetInterface> ActiveWidget : ActiveDisplayWidgets)
		{
			if (ActiveWidget != Widget)
			{
				if (UCommonButtonBase* AsButton = Cast<UCommonButtonBase>(Widget.GetObject()))
				{
					if (AsButton->GetSelected())
					{
						//@todo: Add option here to set sound?
						AsButton->SetIsSelected(false);		
					}
				}
			}
		}
	}

	OnSelectionChange.Broadcast(this, Widget, bGained);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_SelectionChange))
	{
		struct {
			UDisplayScreenComponent* Component;
			const TScriptInterface<IDisplayWidgetInterface>& Widget;
			bool bGained;
		} Args = { this, Widget, bGained };

		ProcessFuncFromResolveMember(Func, &Args);
	}
}

void UDisplayScreenComponent::SetupPanelListViewListeners()
{
	if (bIsPanelListView)
	{
		if (UListView* AsListView = Cast<UListView>(Panel))
		{
			AsListView->OnEntryWidgetGenerated().AddUObject(this, &UDisplayScreenComponent::HandleListViewOnEntryWidgetGenerated);
			AsListView->OnEntryWidgetReleased().AddUObject(this, &UDisplayScreenComponent::HandleListViewOnEntryWidgetReleased);
		}
	}
}

void UDisplayScreenComponent::TeardownPanelListViewListeners()
{
	if (bIsPanelListView)
	{
		if (UListView* AsListView = Cast<UListView>(Panel))
		{
			AsListView->OnEntryWidgetGenerated().RemoveAll(this);
			AsListView->OnEntryWidgetReleased().RemoveAll(this);
		}
	}
}

void UDisplayScreenComponent::HandleListViewOnEntryWidgetGenerated(UUserWidget& GeneratedWidget)
{
	TScriptInterface<IDisplayWidgetInterface> DisplayWidget = &GeneratedWidget;
	DisplayWidgetSetup(DisplayWidget);
	ActiveDisplayWidgets.Add(DisplayWidget);
}

void UDisplayScreenComponent::HandleListViewOnEntryWidgetReleased(UUserWidget& GeneratedWidget)
{
	TScriptInterface<IDisplayWidgetInterface> DisplayWidget = &GeneratedWidget;
	DisplayWidgetTeardown(DisplayWidget);
	ActiveDisplayWidgets.Remove(DisplayWidget);
}