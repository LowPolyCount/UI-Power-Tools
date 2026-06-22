// Copyright (c) Joel Gonzales
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UICSTestHelpers.h"
#include "UICSTestHarness.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"
#include "UI/Screens/UICSScreen.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExecuteActionTest, "UIPowerTools.UICS.Action", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FExecuteActionTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UActionHarness* ActionComponent = UICSTest::CreateComponent<UActionHarness>(Screen);
	TestNotNull("Transaction", ActionComponent);

	UActionTestHarness* Transaction = NewObject<UActionTestHarness>(ActionComponent);
	TestNotNull("Transaction", Transaction);
	ActionComponent->SetActionProvider(Transaction);
	Transaction->bCanTransact = false;
	Transaction->bCanExecuteAction = false;

	TestEqual("bCanTransact", ActionComponent->CanExecuteAction(NewObject<UObjectIntHarness>()), false);
	TestFalse("bCanTransact", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>()));
	TestTrue("bCanTransact", ActionComponent->GetLastActionResult().MatchesTagExact(UICS_Action_Failure));

	Transaction->bCanTransact = true;
	TestEqual("bCanExecuteTransaction", ActionComponent->CanExecuteAction(NewObject<UObjectIntHarness>()), true);
	TestFalse("bCanExecuteTransaction", ActionComponent->ExecuteActionIfAble(NewObject<UObjectIntHarness>()));

	Transaction->bCanExecuteAction = true;
	TestEqual("Can Execute", ActionComponent->CanExecuteAction(NewObject<UObjectIntHarness>()), true);
	TestTrue("Can Execute", ActionComponent->ExecuteActionIfAble(NewObject<UObjectIntHarness>()));
	TestTrue("Can Execute", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>()));
	TestTrue("Can Execute", ActionComponent->GetLastActionResult().MatchesTagExact(UICS_Action_Success));

	Transaction->bCanTransact = false;
	TestEqual("Can't Transact", ActionComponent->CanExecuteAction(NewObject<UObjectIntHarness>()), false);
	TestFalse("Can't Transact", ActionComponent->ExecuteActionIfAble(NewObject<UObjectIntHarness>()));
	TestFalse("Can't Transact", ActionComponent->ExecuteAction(NewObject<UObjectIntHarness>()));
	TestTrue("Can't Transact", ActionComponent->GetLastActionResult().MatchesTagExact(UICS_Action_Failure));

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
	PRAGMA_DISABLE_INTERNAL_WARNINGS
	Action->SetSlot(NewObject<UObjectIntHarness>(), 0);
	Action->SetSlot(NewObject<UObjectIntHarness>(), 1);

	TestEqual("NumSlots", Action->NumSlots(), 2);
	TestTrue("IsSlotValid", Action->IsSlotValid(0));
	TestFalse("IsSlotValid", Action->IsSlotValid(100));
	PRAGMA_ENABLE_INTERNAL_WARNINGS

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
	UActionTestHarness* Action = NewObject<UActionTestHarness>(ActionComponent);
	TestNotNull("Transaction", Action);

	ActionComponent->SetActionProvider(Action);


	UPanelWidget* Panel = UICSTest::SetupViewTest(View, Data);

	ActionComponent->ListenToViewScreenComponent(View);
	UViewWidgetHarness* ViewWidget = Cast<UViewWidgetHarness>(View->GetWidgetAt(0));
	TestNotNull("ViewWidget", ViewWidget);
	ViewWidget->ExecuteTriggeredInput();
	TestEqual("Transactions Complete", ActionComponent->OnCompleteSuccess, 1);

	return true;
}

// test that error tags are generated when we don't have things like Action providers, Linked Action Components, etc.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameplayTagErrors, "UIPowerTools.UICS.Action.GameplayTagErrors", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FGameplayTagErrors::RunTest(const FString& Parameters)
{

	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UViewHarness* View = UICSTest::CreateComponent<UViewHarness>(Screen);
	TestNotNull("View", View);
	UDataHarness* Data = UICSTest::CreateComponent<UDataHarness>(Screen);
	TestNotNull("Data", Data);
	UActionHarness* ActionComponent = UICSTest::CreateComponent<UActionHarness>(Screen);
	TestNotNull("Transaction", ActionComponent);
	UActionTestHarness* Action = NewObject<UActionTestHarness>(ActionComponent);
	TestNotNull("Transaction", Action);

	
	UPanelWidget* Panel = UICSTest::SetupViewTest(View, Data);

	// setup 3 widgets 
	View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(3, View));
	TScriptInterface<IViewWidgetInterface> ViewWidget = View->GetViewWidgetAt(0);


	View->SetLinkedActionScreenComponent(nullptr);
	TestFalse("Cannot Execute ", ViewWidget->Execute_CanExecuteAction(ViewWidget.GetObject()));
	TestTrue("View has no Action Component", ViewWidget->Execute_GetLastActionResult(ViewWidget.GetObject()) == UICS_ACTION_NoActionComponent);
	TestFalse("Cannot Execute ", ViewWidget->Execute_CanExecuteAction(ViewWidget.GetObject()));
	TestTrue("View has no Action Component", ViewWidget->Execute_GetLastActionResult(ViewWidget.GetObject()) == UICS_ACTION_NoActionComponent);


	View->SetLinkedActionScreenComponent(ActionComponent);
	TestFalse("Cannot Execute ", ViewWidget->Execute_CanExecuteAction(ViewWidget.GetObject()));
	TestTrue("View Has No Action Provider", ViewWidget->Execute_GetLastActionResult(ViewWidget.GetObject()) == (UICS_ACTION_NoActionProvider));
	TestFalse("Cannot Execute", ViewWidget->Execute_ExecuteAction(ViewWidget.GetObject()));
	TestTrue("View Has No Action Provider", ViewWidget->Execute_GetLastActionResult(ViewWidget.GetObject()) == (UICS_ACTION_NoActionProvider));



	ActionComponent->SetActionProvider(Action);
	Action->bCanExecuteAction = true;
	TestTrue("Can Execute ", ViewWidget->Execute_CanExecuteAction(ViewWidget.GetObject()));
	TestTrue("Success Tag", ActionComponent->GetLastActionResult() == UICS_Action_Success);
	TestTrue("Can Execute", ViewWidget->Execute_CanExecuteAction(ViewWidget.GetObject()));
	TestTrue("Success Tag", ActionComponent->GetLastActionResult() == UICS_Action_Success);
	

	return true;
}