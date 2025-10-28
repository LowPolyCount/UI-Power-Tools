// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"
#include "Templates/SubclassOf.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Screens/UICS/EntryScreenComponent.h"
#include "UIPowerTools.h"

void UActionScreenComponent::Initialize()
{
	Super::Initialize();

	ListenToViewAction(GetScreenComponentFromSelector<UViewScreenComponent>(ViewToListenTo));
}

bool UActionScreenComponent::IsValidTransaction(UObject* Entry)
{
	bool RetVal = false;
	if (ActionProvider)
	{
		RetVal = ActionProvider->CanExecuteAction(this, Entry);
	}
	OnIsValidResult.Broadcast(this, RetVal);
	return RetVal;
}

ETransactionResult UActionScreenComponent::ExecuteAction(UObject* Entry, bool bPerformCheck)
{
	ETransactionResult RetVal = ETransactionResult::Failure;

	if (ActionProvider && ((bPerformCheck) ? IsValidTransaction(Entry) : true))
	{
		RetVal = ActionProvider->ExecuteAction(this, Entry);
	}

	OnExecuteResult.Broadcast(this, RetVal);
	return RetVal;
}

UObject* UActionScreenComponent::GetSlot(int32 Index) const
{
	UObject* RetVal = nullptr;
	if (IsSlotValid(Index))
	{
		RetVal = Slots[Index];
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("UUICSTransaction::GetSlot() Slot %i was not valid"), Index);
	}
	return RetVal;
}

void UActionScreenComponent::RemoveSlot(int32 Index)
{
	if (IsSlotValid(Index))
	{
		Slots.Remove(Index);
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("RemoveSlot Slot %i was not valid"), Index);
	}
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
		ViewListeningTo->OnAction.RemoveDynamic(this, &UActionScreenComponent::HandleOnAction);
	}

	ViewListeningTo = InView;

	if (ViewListeningTo)
	{
		ViewListeningTo->OnAction.AddUniqueDynamic(this, &UActionScreenComponent::HandleOnAction);
	}
}

void UActionScreenComponent::HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget)
{
	ExecuteAction(Widget->Execute_GetEntryData(Widget.GetObject()), true);
}