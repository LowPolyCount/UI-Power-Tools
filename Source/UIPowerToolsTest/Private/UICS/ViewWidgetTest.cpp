// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UICSTestHarness.h"
#include "TestHelpers.h"
#include "Components/HorizontalBox.h"
#include "UI/Screens/Widgets/ViewUserWidget.h"
#include "UI/Screens/Screen.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FViewWidgetTest, "UIPowerTools.UICS.View.ViewWidget", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FViewWidgetTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);
	UDataHarness* Data = UICSTest::CreateComponent<UDataHarness>(Screen);
	UViewHarness* View = UICSTest::CreateComponent<UViewHarness>(Screen);
	UICSTest::SetupViewTest(View, Data);
	TestNotNull("View", View);
	TestNotNull("Data", Data);

	TScriptInterface<IViewWidgetInterface> AsViewWidgetInterface(View->GetViewWidgetAt(0));
	TestNotNull("AsViewInterface", AsViewWidgetInterface.GetObject());

	// focus test
	// we'll get an error saying that the widget is not owned by a player, so it cannot be focused. Disabling for now. 
	/*TestEqual("SetFocus", AsViewInterface->Execute_IsFocused(ViewWidget), false);
	ViewWidget->UUserWidget::SetFocus();
	TestEqual("SetFocus", AsViewInterface->Execute_IsFocused(ViewWidget), true);
	*/

	// data testing
	{
		TestNotNull("TestData", Data);

		AsViewWidgetInterface->Execute_SetEntryData(AsViewWidgetInterface.GetObject(), 0, Data);

		TestEqual("GetIndex", AsViewWidgetInterface->Execute_GetIndex(AsViewWidgetInterface.GetObject()), 0);
		TestEqual("GetData", AsViewWidgetInterface->Execute_GetEntryData(AsViewWidgetInterface.GetObject()), Cast<UObject>(Data));

		// reset test
		AsViewWidgetInterface->Execute_Reset(AsViewWidgetInterface.GetObject());
		TestEqual("GetIndex", AsViewWidgetInterface->Execute_GetIndex(AsViewWidgetInterface.GetObject()), INDEX_NONE);
		TestNull("Reset", AsViewWidgetInterface->Execute_GetEntryData(AsViewWidgetInterface.GetObject()));
	}

	return true;
}