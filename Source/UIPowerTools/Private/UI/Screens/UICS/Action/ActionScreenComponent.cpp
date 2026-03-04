// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Utility/UIPTStatics.h"
#include "UIPowerTools.h"


UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_CouldNotExecute, "UICS.Action.Could Not Execute", "ExecuteActionIfAble() was called and The Action provider's CanExecuteAction() returned false");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_Success, "UICS.Action.Success", "Executed Action Successfully");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_Failure, "UICS.Action.Failure", "Was not able to execute action");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UICS_Action_Async, "UICS.Action.Async", "Action is waiting for an asynchronous callback");

void UActionScreenComponent::Initialize()
{
	Super::Initialize();

	ListenToViewScreenComponent(UUIPTStatics::GetScreenComponentFromSelector<UViewScreenComponent>(this, ViewToListenTo));
}

bool UActionScreenComponent::IsValidTransaction(UObject* Entry)
{
	return CanExecuteAction(Entry);
}

bool UActionScreenComponent::CanExecuteAction(UObject* Entry)
{
	bool RetVal = false;
	if (ActionProvider)
	{
		RetVal = ActionProvider->CanExecuteAction(this, Entry);
	}

	return RetVal;
}

FGameplayTag UActionScreenComponent::ExecuteActionIfAble(UObject* Entry)
{
	FGameplayTag RetVal = UICS_Action_CouldNotExecute;

	if (ActionProvider && CanExecuteAction(Entry))
	{
		RetVal = ExecuteAction(Entry);
	}
	return RetVal;
}

FGameplayTag UActionScreenComponent::ExecuteAction(UObject* Entry)
{
	FGameplayTag RetVal = UICS_Action_Failure;

	if (ActionProvider)
	{
		RetVal = ActionProvider->ExecuteAction(this, Entry);
	}

	OnExecuteResult.Broadcast(this, RetVal);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_OnExecuteResult))
	{
		struct {
			UActionScreenComponent* Component;
			FGameplayTag Result;
		} Args = { this, RetVal };

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

void UActionScreenComponent::SetActionTriggers(EActionTriggers InActionTriggers)
{
	UViewScreenComponent* CurrentView = ViewListeningTo;

	// reset listeners
	RemoveCurrentViewScreenComponent();
	ActionTriggers = static_cast<int32>(InActionTriggers);
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
		if (EnumHasAnyFlags(static_cast<EActionTriggers>(ActionTriggers), EActionTriggers::Focus))
		{
			ViewListeningTo->OnFocusChange.RemoveDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
		}
		if (EnumHasAnyFlags(static_cast<EActionTriggers>(ActionTriggers), EActionTriggers::Hover))
		{
			ViewListeningTo->OnHoverChange.RemoveDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
		}
		if (EnumHasAnyFlags(static_cast<EActionTriggers>(ActionTriggers), EActionTriggers::Input))
		{
			ViewListeningTo->OnInputAction.RemoveDynamic(this, &UActionScreenComponent::HandleOnActionTrigger);
		}
		
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

			if (EnumHasAnyFlags(static_cast<EActionTriggers>(ActionTriggers), EActionTriggers::Focus))
			{
				ViewListeningTo->OnFocusChange.AddDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
			}
			if (EnumHasAnyFlags(static_cast<EActionTriggers>(ActionTriggers), EActionTriggers::Hover))
			{
				ViewListeningTo->OnHoverChange.AddDynamic(this, &UActionScreenComponent::HandleOnActionTriggerGain);
			}
			if (EnumHasAnyFlags(static_cast<EActionTriggers>(ActionTriggers), EActionTriggers::Input))
			{
				ViewListeningTo->OnInputAction.AddDynamic(this, &UActionScreenComponent::HandleOnActionTrigger);
			}
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