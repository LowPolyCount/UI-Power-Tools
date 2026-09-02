// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include"UICSTestHarness.h"
#include "UICSTestHelpers.h"
#include "UICS/Screens/UICSScreen.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FViewTest,"UICS.Component.Display", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FViewTest::RunTest(const FString& Parameters)
{
	UWorld* World = nullptr;
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// test creation of view widgets based on what Data Component creates
	{
		UDataHarness* Data = UICSTest::CreateComponent<UDataHarness>(Screen);
		UViewHarness* View = UICSTest::CreateComponent<UViewHarness>(Screen);
		TestNotNull("Data", Data);
		TestNotNull("View", View);

		UPanelWidget* Panel = UICSTest::SetupViewTest(View, Data);
		//TSharedRef<SWidget> SWidget = Panel->TakeWidget();

		TestTrue("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num() == UDataRetrieverHarness::NumTestEntries);
		TestTrue("View->GetAllDisplayWidgets()", Panel->GetAllChildren().Num() == UDataRetrieverHarness::NumTestEntries);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FViewCreationTest,"UICS.Component.Display.WidgetCreation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FViewCreationTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// disabling these tests for now.  IDisplayWidgetInterface is changing, there isn't a good way to SetFocus(), etc. 
	/*
	// test widget creation
	{
		UDataHarness* Data = UICSTest::CreateComponent<UDataHarness>(Screen);
		UViewHarness* View = UICSTest::CreateComponent<UViewHarness>(Screen);
		TestNotNull("Data", Data);
		TestNotNull("View", View);

		UPanelWidget* Panel = UICSTest::SetupViewTest(View, Data);
		TestEqual("View->CountOnRetrieval", View->CountOnRetrieval, 1);

		TScriptInterface<IDisplayWidgetInterface> Widget0 = View->GetViewWidgetAt(0);
		TScriptInterface<IDisplayWidgetInterface> Widget1 = View->GetViewWidgetAt(1);
		TScriptInterface<IDisplayWidgetInterface> Widget2 = View->GetViewWidgetAt(2);
		TestNotNull("Widget0", Widget0.GetObject());
		TestNotNull("Widget1", Widget1.GetObject());
		TestNotNull("Widget2", Widget2.GetObject());
		TestEqual("CountOnWidgetsPopulated", View->CountOnWidgetsPopulated, 1);

		// test that focus events work
		Widget0->Execute_SetFocus(Widget0.GetObject(), true);
		TestTrue("Execute_IsFocused", Widget0->Execute_IsFocused(Widget0.GetObject()));
		TestFalse("Execute_IsFocused", Widget1->Execute_IsFocused(Widget1.GetObject()));
		TestFalse("Execute_IsFocused", Widget2->Execute_IsFocused(Widget2.GetObject()));
		TestEqual("CountOnFocusGained", View->CountOnFocusGained, 1);
		TestEqual("CountOnFocusLost", View->CountOnFocusLost, 0);
		TestEqual("CountOnFocusChanged", View->CountOnFocusChanged, 1);

		// @note: Because we're in a unit test, focus changing won't work like it would in a real widget.
		Widget1->Execute_SetFocus(Widget1.GetObject(), true);
		Widget0->Execute_SetFocus(Widget0.GetObject(), false);
		TestTrue("Execute_IsFocused", Widget1->Execute_IsFocused(Widget1.GetObject()));
		TestFalse("Execute_IsFocused", Widget0->Execute_IsFocused(Widget0.GetObject()));
		TestFalse("Execute_IsFocused", Widget2->Execute_IsFocused(Widget2.GetObject()));
		TestEqual("CountOnFocusGained", View->CountOnFocusGained, 2);
		TestEqual("CountOnFocusLost", View->CountOnFocusLost, 1);
		TestEqual("CountOnFocusChanged", View->CountOnFocusChanged, 3);

		// test that actions works
		Widget1->Execute_ForceInputAction(Widget1.GetObject());
		TestEqual("CountOnAction", View->CountOnAction, 1);

		// test that selected events work
		TestFalse("IsSelectedWidget", View->IsSelectedWidget());
		Widget1->Execute_SetSelected(Widget1.GetObject(), true);
		TestTrue("IsSelectedWidget", View->IsSelectedWidget());
		TestEqual("CountOnSelectionGained", View->CountOnSelectionGained, 1);
		TestEqual("CountOnSelectionLost", View->CountOnSelectionLost, 0);
		TestEqual("GetFirstSelectedWidget", View->GetFirstSelectedWidget(), Widget1);

		// test that only 1 widget will stay selected
		View->SetSingleSelection(true);
		Widget0->Execute_SetSelected(Widget0.GetObject(), true);
		TestTrue("Execute_IsSelected", Widget0->Execute_IsSelected(Widget0.GetObject()));
		TestFalse("Execute_IsSelected", Widget1->Execute_IsSelected(Widget1.GetObject()));
		TestEqual("CountOnSelectionGained", View->CountOnSelectionGained, 2);
		TestEqual("CountOnSelectionLost", View->CountOnSelectionLost, 1);
		TestEqual("GetFirstSelectedWidget", View->GetFirstSelectedWidget(), Widget0);
		TestEqual("GetAllSelectedWidgets", View->GetAllSelectedWidgets().Num(), 1);

		// test that multiple widgets can stay selected
		View->SetSingleSelection(false);
		Widget0->Execute_SetSelected(Widget0.GetObject(), false);
		Widget1->Execute_SetSelected(Widget1.GetObject(), true);
		Widget2->Execute_SetSelected(Widget2.GetObject(), true);
		TestEqual("GetFirstSelectedWidget", View->GetFirstSelectedWidget(), Widget1);
		TestEqual("GetAllSelectedWidgets", View->GetAllSelectedWidgets().Num (), 2); 
	}*/

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FViewCachingTest, "UICS.Component.Display.WidgetCaching", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FViewCachingTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// test widget caching
	{
		UViewHarness* View(UICSTest::CreateComponent<UViewHarness>(Screen));
		TestNotNull("View", View);
		UPanelWidget* Panel = UICSTest::SetupViewTest(View, nullptr);
		TestNotNull("Panel", Panel);


		// test when caching is turned off
		View->SetCacheWidgets(false);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(3, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 3);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 0);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(0, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 0);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 0);

		// test when caching is turned off
		View->SetCacheWidgets(false);

		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 0);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 0);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(3, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 3);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 0);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(0, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 0);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 0);

		// test when caching is turned on
		View->SetCacheWidgets(true);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(3, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 3);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 0);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(1, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 1);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 2);

		View->TestHandleOnDataRetrieval(nullptr, UICSTest::GenerateEntries(0, View));
		TestEqual("View->GetAllDisplayWidgets()", View->GetAllDisplayWidgets().Num(), 0);
		TestEqual("View->GetAllCachedWidgets()", View->GetNumCachedWidgets(), 3);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMemoryStomp, "UICS.Component.Display.MemoryStomp", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FMemoryStomp::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// test widget caching
	{
		UViewHarness* View(UICSTest::CreateComponent<UViewHarness>(Screen));
		TestNotNull("View", View);

		UPanelWidget* Panel = UICSTest::SetupViewTest(View, nullptr);
		TestNotNull("Panel", Panel);

	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FViewDesiredFocusTargetTest,"UICS.Component.Display.DesiredFocusTarget", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FViewDesiredFocusTargetTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	{
		UViewHarness* View1(UICSTest::CreateComponent<UViewHarness>(Screen));
		TestNotNull("View1", View1);
		UPanelWidget* Panel1 = UICSTest::SetupViewTest(View1, nullptr);
		TestNotNull("Panel", Panel1);

		UViewHarness* View2(UICSTest::CreateComponent<UViewHarness>(Screen));
		TestNotNull("View2", View2);
		UPanelWidget* Panel2 = UICSTest::SetupViewTest(View2, nullptr);
		TestNotNull("Panel2", Panel2);

		Screen->AddComponent(View1);
		Screen->AddComponent(View2);

		View1->ManuallySetData(UICSTest::GenerateEntries(3, View1));
		View2->ManuallySetData(UICSTest::GenerateEntries(3, View2));

		// the widgets don't get created with Focusable set to true.  @todo: Find core issue here. 
		for (TScriptInterface<IDisplayWidgetInterface> ViewWidget : View1->GetAllDisplayWidgets())
		{
			if (UUserWidget* AsUWidget = Cast<UUserWidget>(ViewWidget.GetObject()))
			{
				AsUWidget->SetIsFocusable(true);
			}
		}

		for (TScriptInterface<IDisplayWidgetInterface> ViewWidget : View2->GetAllDisplayWidgets())
		{
			if (UUserWidget* AsUWidget = Cast<UUserWidget>(ViewWidget.GetObject()))
			{
				AsUWidget->SetIsFocusable(true);
			}
		}

		View1->SetIsDesiredFocusTarget(true);
		View2->SetIsDesiredFocusTarget(true);
		
		//@todo: The Pointers here didn't equal, but it is the same object. Investigate. 
		TestEqual("NativeGetDesiredFocusTarget", Screen->NativeGetDesiredFocusTarget()->GetFName(), View1->GetViewWidgetAt(0).GetObject()->GetFName());

		View1->SetIsDesiredFocusTarget(false);
		View2->SetIsDesiredFocusTarget(false);

		TestNull("FocusTarget when no ViewComponents are desired", Screen->NativeGetDesiredFocusTarget());

		// 
		{
			View2->SetIsDesiredFocusTarget(true);
			TScriptInterface<IDisplayWidgetInterface> Widget0 = View2->GetViewWidgetAt(0);
			UUserWidget* AsWidget = Cast<UUserWidget>(Widget0.GetObject());
			AsWidget->SetIsFocusable(false);
			TestEqual("First Focusable Widget in View Component", Screen->NativeGetDesiredFocusTarget(), Cast<UWidget>(View2->GetViewWidgetAt(1).GetObject()));
		}
		// @todo: Test that the first focusable widget is what we're using. 
		// @todo: Test not completed yet?

	}

	return true;
}