// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Components/Display/DisplayScreenComponent.h"
#include "UICSModule.h"
#include "UObject/UObjectGlobals.h"
#include "UICS/Screens/Components/Data//DataScreenComponent.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "CommonButtonBase.h"
#include "Components/UniformGridPanel.h"
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
		Panel = Cast<UPanelWidget>(PanelSelector.GetWidget(Cast<UWidget>(UUIPTStatics::GetScreenAccessor(this).GetObject())));

		// show preview of widgets in design view
		if (bIsDesignTime)
		{
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
				if (Panel->HasChild(AsUWidget))
				{
					Panel->RemoveChild(AsUWidget);
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
	for(int32 i= ActiveDisplayWidgets.Num()-1; i >= 0; --i)
	{
		RemoveViewWidget(ActiveDisplayWidgets[i]);
	}
	ActiveDisplayWidgets.Empty();

	for (FCachedWidget& Widget : CachedWidgets)
	{
		Widget.UserWidget->Release();
	}
	CachedWidgets.Empty();
	//check(ActiveDisplayWidgets.Num() == 0);

	Super::NativeDestruct();

}

#if WITH_EDITOR
void UDisplayScreenComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	for (FProperty* Property : PropertyChangedEvent.PropertyChain)
	{
		if (Property->GetName() == GET_MEMBER_NAME_CHECKED(UDisplayScreenComponent, PanelSelector))
		{
			// @TODO: PanelSelector.WidgetClassName isn't coming through correctly so for now we're setting bPanelIsAGrid = true
			//bPanelIsAGrid = (PanelSelector.WidgetClassName == UUniformGridPanel::StaticClass()->GetFName());
			
		}
	}
}
#endif

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

TScriptInterface<IDisplayWidgetInterface> UDisplayScreenComponent::GetViewWidgetAt(int32 Index) const
{ 
	TScriptInterface<IDisplayWidgetInterface> RetVal;

	if(ActiveDisplayWidgets.IsValidIndex(Index))
	{
		RetVal = ActiveDisplayWidgets[Index];
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No Widget in Index %i"), Index);
	}

	return RetVal;
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

	TScriptInterface<IDisplayWidgetInterface> ViewWidget = GetViewWidgetAt(Index);

	if (UUserWidget* AsUserWidget = Cast<UUserWidget>(ViewWidget.GetObject()))
	{
		RetVal = AsUserWidget;
	}

	return RetVal;
}

bool UDisplayScreenComponent::IsSelectedWidget() const
{
	return static_cast<bool>(GetFirstSelectedWidget());
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
			RemoveViewWidget(ActiveDisplayWidgets[i]);
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
		if (ActiveDisplayWidgets.IsValidIndex(i))
		{
			
			/*if (Panel)
			{
				AddToPanel(ActiveDisplayWidgets[i]);
			}*/

			ActiveDisplayWidgets[i]->Execute_Reset(ActiveDisplayWidgets[i].GetObject());
			ActiveDisplayWidgets[i]->Execute_SetEntryData(ActiveDisplayWidgets[i].GetObject(), i, Entries[i]);
		}
		else
		{
			UE_LOG(LogUICS, Warning, TEXT(" There are %i Entries but only %i ActiveDisplayWidgets. There is no widget for Entry %i"), Entries.Num(), ActiveDisplayWidgets.Num(), i);
		}
	}

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
		ViewWidgetSetup(RetVal);
		AddToPanel(RetVal);

		// we can't remove the entry from CachedWidgets until after AddToPanel() otherwise the slate widget will be destroyed
		if (FoundCachedWidgetIndex != INDEX_NONE)
		{
			CachedWidgets.RemoveAt(FoundCachedWidgetIndex);
		}
	}

	return RetVal;
}

void UDisplayScreenComponent::AddToPanel(TScriptInterface<IDisplayWidgetInterface>& Widget)
{
	if (Panel)
	{
		if (UUniformGridPanel* AsGrid = Cast<UUniformGridPanel>(Panel))
		{
			AsGrid->AddChildToUniformGrid(Cast<UWidget>(Widget.GetObject()), GetNumWidgets() / ColumnsGridWillHave, GetNumWidgets() % ColumnsGridWillHave);
		}
		else
		{
			Panel->AddChild(Cast<UUserWidget>(Widget.GetObject()));
		}
	}
}

void UDisplayScreenComponent::RemoveViewWidget(TScriptInterface<IDisplayWidgetInterface> Widget)
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

			ActiveDisplayWidgets.Remove(Widget);

			AsUserWidget->RemoveFromParent();
		}
	}
}

void UDisplayScreenComponent::ViewWidgetSetup(TScriptInterface<IDisplayWidgetInterface> Widget)
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

void UDisplayScreenComponent::ViewWidgetTeardown(TScriptInterface<IDisplayWidgetInterface> Widget)
{
	if (Widget)
	{
		Widget->SetOwningDisplayScreenComponent(nullptr);
		Widget->Release();
		Widget->GetOnAction().RemoveDynamic(this, &UDisplayScreenComponent::HandleWidgetOnAction);
		Widget->GetOnSelectionChange().RemoveDynamic(this, &UDisplayScreenComponent::HandleWidgetOnSelectionChange);
		Widget->GetOnFocusChange().RemoveDynamic(this, &UDisplayScreenComponent::HandleWidgetOnFocusChange);
		Widget->GetOnHoverChange().AddUniqueDynamic(this, &UDisplayScreenComponent::HandleWidgetOnHoverChange);
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