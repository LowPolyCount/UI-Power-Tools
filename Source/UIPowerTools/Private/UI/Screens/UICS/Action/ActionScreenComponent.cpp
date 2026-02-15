// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Utility/UIPTStatics.h"
#include "UIPowerTools.h"

void UActionScreenComponent::Initialize()
{
	Super::Initialize();

	ListenToViewAction(UUIPTStatics::GetScreenComponentFromSelector<UViewScreenComponent>(this, ViewToListenTo));
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

EActionResult UActionScreenComponent::ExecuteActionIfAble(UObject* Entry)
{
	EActionResult RetVal = EActionResult::CouldNotExecute;

	if (ActionProvider && CanExecuteAction(Entry))
	{
		RetVal = ExecuteAction(Entry);
	}
	return RetVal;
}

EActionResult UActionScreenComponent::ExecuteAction(UObject* Entry)
{
	EActionResult RetVal = EActionResult::Failure;

	if (ActionProvider)
	{
		RetVal = ActionProvider->ExecuteAction(this, Entry);
	}

	OnExecuteResult.Broadcast(this, RetVal);

	if (UFunction* Func = ResolveMemberReference(BindableEvents.Bind_OnExecuteResult))
	{
		struct {
			UActionScreenComponent* Component;
			EActionResult Result;
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
	if (ViewListeningTo)
	{
		ViewListeningTo->OnInputAction.RemoveDynamic(this, &UActionScreenComponent::HandleOnInputAction);
	}

	ViewListeningTo = InView;

	if (ViewListeningTo)
	{
		ViewListeningTo->OnInputAction.AddUniqueDynamic(this, &UActionScreenComponent::HandleOnInputAction);
	}
}

void UActionScreenComponent::HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget)
{
	HandleOnInputAction(Component, Widget);
}

void UActionScreenComponent::HandleOnInputAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget)
{
	if (Widget)
	{
		ExecuteActionIfAble(Widget->Execute_GetEntryData(Widget.GetObject()));
	}
}