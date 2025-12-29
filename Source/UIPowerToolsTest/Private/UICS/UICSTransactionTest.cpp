// Copyright (c) Joel Gonzales
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestHelpers.h"
#include "UICSTestHarness.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"
#include "UI/Screens/Screen.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExecuteActionTest, "UIPowerTools.UICS.Action", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FExecuteActionTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UActionHarness* ActionComponent = UICSTest::CreateComponent<UActionHarness>(Screen);
	TestNotNull("Transaction", ActionComponent);

	UActionTestHarness* Transaction = NewObject<UActionTestHarness>(ActionComponent);
	TestNotNull("Transaction", Transaction);
	ActionComponent->SetTransactor(Transaction);
	Transaction->bCanTransact = false;
	Transaction->bCanExecuteAction = false;

	TestEqual("bCanTransact", ActionComponent->IsValidTransaction(NewObject<UObjectIntHarness>()), false);
	TestEqual("bCanTransact", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>(), true), ETransactionResult::Failure);

	Transaction->bCanTransact = true;
	TestEqual("bCanExecuteTransaction", ActionComponent->IsValidTransaction(NewObject<UObjectIntHarness>()), true);
	TestEqual("bCanExecuteTransaction", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>(), true), ETransactionResult::Failure);

	Transaction->bCanExecuteAction = true;
	TestEqual("Can Execute", ActionComponent->IsValidTransaction(NewObject<UObjectIntHarness>()), true);
	TestEqual("Can Execute", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>(), true), ETransactionResult::Success);
	TestEqual("Can Execute", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>(), false), ETransactionResult::Success);

	Transaction->bCanTransact = false;
	TestEqual("Can't Transact", ActionComponent->IsValidTransaction(NewObject<UObjectIntHarness>()), false);
	TestEqual("Can't Transact", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>(), true), ETransactionResult::Failure);
	TestEqual("Can't Transact", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>(), false), ETransactionResult::Success);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSlotTest, "UIPowerTools.UICS.Action.Slot", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FSlotTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UActionHarness* Action = UICSTest::CreateComponent<UActionHarness>(Screen);
	TestNotNull("Transaction", Action);

	// Test Set Get Slot
	Action->SetSlot(NewObject<UObjectIntHarness>(), 0);
	Action->SetSlot(NewObject<UObjectIntHarness>(), 1);
	TestEqual("NumSlots", Action->NumSlots(), 2);
	TestTrue("IsSlotValid", Action->IsSlotValid(0));
	TestFalse("IsSlotValid", Action->IsSlotValid(100));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FListeningToViewAction, "UIPowerTools.UICS.Action.ListeningToViewAction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FListeningToViewAction::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UViewHarness* View = UICSTest::CreateComponent<UViewHarness>(Screen);
	TestNotNull("View", View);
	UDataHarness* Data = UICSTest::CreateComponent<UDataHarness>(Screen);
	TestNotNull("Data", Data);
	UActionHarness* ActionComponent = UICSTest::CreateComponent<UActionHarness>(Screen);
	TestNotNull("Transaction", ActionComponent);
	UActionTestHarness* Transaction = NewObject<UActionTestHarness>(ActionComponent);
	TestNotNull("Transaction", Transaction);
	ActionComponent->SetTransactor(Transaction);


	UPanelWidget* Panel = UICSTest::SetupViewTest(View, Data);

	ActionComponent->ListenToViewAction(View);
	TScriptInterface<IViewWidgetInterface> ViewWidget = View->GetViewWidgetAt(0);
	TestNotNull("ViewWidget", ViewWidget.GetObject());
	ViewWidget->Execute_ForceInputAction(ViewWidget.GetObject());
	TestEqual("Transactions Complete", ActionComponent->OnCompleteSuccess, 1);

	return true;
}