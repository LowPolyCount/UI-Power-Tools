// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Utility/UIPTStatics.h"
#include "UIPowerTools.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_ACTION_Default, "UICS.Action.Default", "Variable was not set");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_CouldNotExecute, "UICS.Action.CouldNotExecute", "ExecuteActionIfAble() was called and The Action provider's CanExecuteAction() returned false");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_Success, "UICS.Action.Success", "Executed Action Successfully");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_Failure, "UICS.Action.Failure", "Was not able to execute action");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_Async, "UICS.Action.Async", "Action is waiting for an asynchronous callback");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_ACTION_NoActionComponent, "UICS.Action.NoActionComponent", "IViewWidget's owning View Screen Component does not have an Action Screen Component linked to it.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_ACTION_NoActionProvider, "UICS.Action.NoActionProvider", "The Action Screen Component does not have an action provider");


void UActionScreenComponent::Initialize()
{
	Super::Initialize();

	if (ActionProvider)
	{
		ActionProvider->Initialize(this);
	}
	ListenToViewScreenComponent(UUIPTStatics::GetScreenComponentFromSelector<UViewScreenComponent>(this, ViewToListenTo));
}

bool UActionScreenComponent::IsValidTransaction(UObject* Entry)
{
	return CanExecuteAction(Entry);
}

FGameplayTag UActionScreenComponent::GetLastActionResult() const
{
	FGameplayTag RetVal = UICS_ACTION_NoActionProvider;
	if (ActionProvider)
	{
		RetVal = ActionProvider->GetLastActionResult();
	}
	return RetVal;
}

bool UActionScreenComponent::CanExecuteAction(UObject* Entry)
{
	bool RetVal = false;
	if (ActionProvider)
	{
		RetVal = ActionProvider->CanExecuteAction(Entry);
	}

	return RetVal;
}

bool UActionScreenComponent::ExecuteActionIfAble(UObject* Entry)
{
	bool RetVal = false;
	if (ActionProvider && CanExecuteAction(Entry))
	{
		RetVal = ExecuteAction(Entry);
	}
	return RetVal;
}

bool UActionScreenComponent::ExecuteAction(UObject* Entry)
{
	bool RetVal = false;
	
	FGameplayTag ResultsTag = UICS_ACTION_NoActionProvider;
	if (ActionProvider)
	{
		RetVal = ActionProvider->ExecuteAction(Entry);
		ResultsTag = ActionProvider->GetLastActionResult();
	}

	OnActionExecuteResult.Broadcast(this, RetVal, ResultsTag);
	PRAGMA_DISABLE_INTERNAL_WARNINGS
	OnExecuteResult_DEPRECATED.Broadcast(this, ResultsTag);
	PRAGMA_ENABLE_INTERNAL_WARNINGS

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_OnActionExecuteResult))
	{
		struct {
			UActionScreenComponent* Component;
			bool bDidExecuteSucceed;
			FGameplayTag ResultTag;
		} Args = { this, RetVal, ResultsTag };

		ProcessFuncFromResolveMember(Func, &Args);
	}
	return RetVal;
}

UObject* UActionScreenComponent::GetSlot(int32 Index) const
{
	UObject* RetVal = nullptr;
	PRAGMA_DISABLE_INTERNAL_WARNINGS
	if (IsSlotValid(Index))
	{
		RetVal = Slots[Index];
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("UUICSTransaction::GetSlot() Slot %i was not valid"), Index);
	}
	PRAGMA_ENABLE_INTERNAL_WARNINGS
	return RetVal;
}

void UActionScreenComponent::RemoveSlot(int32 Index)
{
	PRAGMA_DISABLE_INTERNAL_WARNINGS
	if (IsSlotValid(Index))
	{
		Slots.Remove(Index);
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("RemoveSlot Slot %i was not valid"), Index);
	}
	PRAGMA_ENABLE_INTERNAL_WARNINGS
}

void UActionScreenComponent::SetSlot(UObject* InEntry, int32 Index)
{
	Slots.Add(Index, InEntry);
}

bool UActionScreenComponent::IsSlotValid(int32 Index) const
{
	return Slots.Find(Index) != nullptr;
}

int32 UActionScreenComponent::NumSlots() const
{
	return Slots.Num();
}

void UActionScreenComponent::ListenToViewAction(UViewScreenComponent* InView)
{
	ListenToViewScreenComponent(InView);
}

void UActionScreenComponent::SetTriggerOnGainsFocus(bool bInTriggerOnGainsFocus) 
{
	UViewScreenComponent* CurrentView = ViewListeningTo;
	RemoveCurrentViewScreenComponent();
	bTriggerOnGainsFocus = bInTriggerOnGainsFocus; 
	SetupListenersToViewScreenComponent(CurrentView);
}

void UActionScreenComponent::SetTriggerOnHover(bool bInTriggerOnHover) 
{
	UViewScreenComponent* CurrentView = ViewListeningTo;
	RemoveCurrentViewScreenComponent();
	bTriggerOnHover = bInTriggerOnHover;
	SetupListenersToViewScreenComponent(CurrentView);
}

void UActionScreenComponent::SetTriggerOnInputAction(bool bInTriggerOnInputAction) 
{
	UViewScreenComponent* CurrentView = ViewListeningTo;
	RemoveCurrentViewScreenComponent();
	bTriggerOnInputAction = bInTriggerOnInputAction; 
	SetupListenersToViewScreenComponent(CurrentView);
}

void UActionScreenComponent::ListenToViewScreenComponent(UViewScreenComponent* InView)
{
	RemoveCurrentViewScreenComponent();
	SetupListenersToViewScreenComponent(InView);
}

void UActionScreenComponent::RemoveCurrentViewScreenComponent()
{
	if (ViewListeningTo)
	{
		if (bTriggerOnGainsFocus)
		{
			ViewListeningTo->OnFocusChange.RemoveDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
		}
		if (bTriggerOnHover)
		{
			ViewListeningTo->OnHoverChange.RemoveDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
		}
		if (bTriggerOnInputAction)
		{
			ViewListeningTo->OnInputAction.RemoveDynamic(this, &UActionScreenComponent::HandleOnActionTrigger);
		}

		ViewListeningTo->SetLinkedActionScreenComponent(nullptr);
		ViewListeningTo = nullptr;
	}
}

void UActionScreenComponent::SetupListenersToViewScreenComponent(UViewScreenComponent* InView)
{
	if (!ViewListeningTo)
	{
		if (InView)
		{
			ViewListeningTo = InView;

			if (bTriggerOnGainsFocus)
			{
				ViewListeningTo->OnFocusChange.AddDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
			}
			if (bTriggerOnHover)
			{
				ViewListeningTo->OnHoverChange.AddDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
			}
			if (bTriggerOnInputAction)
			{
				ViewListeningTo->OnInputAction.AddDynamic(this, &UActionScreenComponent::HandleOnActionTrigger);
			}

			ViewListeningTo->SetLinkedActionScreenComponent(this);
		}
		else
		{
			// @todo: add logging
		}
	}
	else
	{
		// @todo: add logging
	}
}

void UActionScreenComponent::HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget)
{
	HandleOnActionTrigger(Component, Widget);
}

void UActionScreenComponent::HandleOnActionTrigger(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget)
{
	if (Widget)
	{
		ExecuteActionIfAble(Widget->Execute_GetEntryData(Widget.GetObject()));
	}
}

void UActionScreenComponent::HandleOnActionTriggerGain(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained)
{
	if (bGained && Widget)
	{
		ExecuteActionIfAble(Widget->Execute_GetEntryData(Widget.GetObject()));
	}
}