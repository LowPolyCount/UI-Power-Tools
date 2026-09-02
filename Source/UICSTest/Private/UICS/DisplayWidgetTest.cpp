// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UICSTestHarness.h"
#include "UICSTestHelpers.h"
#include "UICS/Screens/Components/Display/DisplayUserWidget.h"

// @TODO: some things to make the tests easier:
// Get a game Instance using FActorTestSpawn - call Spawner.InitializeGameSubsystems() first.
// can create a viewport client for these tests, set the viewport overlay widget for the viewport client. Create a new SOverlay, then call `SetViewportOverlayWidget`

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDisplayWidgetTest, "UICS.Component.Display.DisplayWidget", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FDisplayWidgetTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UDataHarness* Data = UICSTest::CreateComponent<UDataHarness>(Screen);
	UViewHarness* View = UICSTest::CreateComponent<UViewHarness>(Screen);
	UICSTest::SetupViewTest(View, Data);
	TestNotNull("View", View);
	TestNotNull("Data", Data);

	TScriptInterface<IDisplayWidgetInterface> AsDisplayWidgetInterface(View->GetViewWidgetAt(0));
	TestNotNull("AsViewInterface", AsDisplayWidgetInterface.GetObject());

	// focus test
	// we'll get an error saying that the widget is not owned by a player, so it cannot be focused. Disabling for now. 
	/*TestEqual("SetFocus", AsViewInterface->Execute_IsFocused(ViewWidget), false);
	ViewWidget->UUserWidget::SetFocus();
	TestEqual("SetFocus", AsViewInterface->Execute_IsFocused(ViewWidget), true);
	*/

	// data testing
	{
		TestNotNull("TestData", Data);

		AsDisplayWidgetInterface->Execute_SetEntryData(AsDisplayWidgetInterface.GetObject(), 0, Data);

		TestEqual("GetIndex", AsDisplayWidgetInterface->Execute_GetIndex(AsDisplayWidgetInterface.GetObject()), 0);
		TestEqual("GetData", AsDisplayWidgetInterface->Execute_GetEntryData(AsDisplayWidgetInterface.GetObject()), Cast<UObject>(Data));

		// reset test
		AsDisplayWidgetInterface->Execute_Reset(AsDisplayWidgetInterface.GetObject());
		TestEqual("GetIndex", AsDisplayWidgetInterface->Execute_GetIndex(AsDisplayWidgetInterface.GetObject()), INDEX_NONE);
		TestNull("Reset", AsDisplayWidgetInterface->Execute_GetEntryData(AsDisplayWidgetInterface.GetObject()));
	}

	return true;
}