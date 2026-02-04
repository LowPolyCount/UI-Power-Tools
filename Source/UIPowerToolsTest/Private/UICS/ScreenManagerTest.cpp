// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UICSTestHarness.h"
#include "TestHelpers.h"
#include "Components/HorizontalBox.h"
#include "UI/Screens/UICSScreen.h"
#include "UI/Screens/ScreenManager.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "UI/Tools/UIPowerToolsDeveloperSettings.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScreenManagerAddRemoveTest, "UIPowerTools.UICS.ScreenManager.AddRemove", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FScreenManagerAddRemoveTest::RunTest(const FString& Parameters)
{
	// @note we get warnings saying that UScreenHarness doesn't have a world. But we don't want to create a world here 
	//			because that will overwrite the world in the editor. 
	UScreenManager* ScreenManager = NewObject<UScreenManager>();
	TestNotNull("ScreenManager", ScreenManager);
	

	// test screen adding functionality like visibility, etc. 
	{
		// @note turn off warnings saying that UScreenHarness doesn't have a world. Creating a world here will overwrite the world in the editor.
		
		UScreenHarness* Screen0 = NewObject<UScreenHarness>(ScreenManager);
		UScreenHarness* Screen1 = NewObject<UScreenHarness>(ScreenManager);
		UScreenHarness* Screen2 = NewObject<UScreenHarness>(ScreenManager);
		UScreenHarness* Screen3 = NewObject<UScreenHarness>(ScreenManager);
		

		TSubclassOf<UUserWidget> AsSubclassOf(UScreenHarness::StaticClass());

		TestFalse("IsScreenOfClassOnStack", ScreenManager->IsScreenOfClassOnStack(AsSubclassOf));
		
		// @note turn off warnings saying that UScreenHarness doesn't have a world. Creating a world here will overwrite the world in the editor.
		// @todo: We could make a ScreenManager Harness and override AddToScreen()
		bSuppressLogWarnings = true;
		ScreenManager->AddScreen(Screen0);
		ScreenManager->AddScreen(Screen1);
		ScreenManager->AddScreen(Screen2);
		bSuppressLogWarnings = false;

		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible );
		TestEqual("NumScreens", ScreenManager->NumScreens(), 3);
		TestTrue("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen0));
		TestFalse("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen3));
		TestTrue("IsScreenOfClassOnStack", ScreenManager->IsScreenOfClassOnStack(AsSubclassOf));

		bSuppressLogWarnings = true;
		ScreenManager->AddScreen(Screen3, false);
		bSuppressLogWarnings = false;

		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen1->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("Visibility", Screen3->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 4);

		Screen3->RemoveFromParent();
		
		
		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen1->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 3);

		// close screen out of order
		Screen1->RemoveFromParent();

		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 2);

		Screen2->RemoveFromParent();

		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 1);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScreenManagerDoubleAddTest, "UIPowerTools.UICS.ScreenManager.DoubleAdd", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FScreenManagerDoubleAddTest::RunTest(const FString& Parameters)
{
	UScreenManager* ScreenManager = NewObject<UScreenManager>();
	TestNotNull("ScreenManager", ScreenManager);

	// test adding same screen twice
	{
		
		
		UScreenHarness* Screen0 = NewObject<UScreenHarness>();
		UScreenHarness* Screen1 = NewObject<UScreenHarness>();
		
		// @note turn off warnings saying that UScreenHarness doesn't have a world. Creating a world here will overwrite the world in the editor.
		// @todo: We could make a ScreenManager Harness and override AddToScreen()
		bSuppressLogWarnings = true;
		ScreenManager->AddScreen(Screen0);
		ScreenManager->AddScreen(Screen1);
		ScreenManager->AddScreen(Screen0);
		bSuppressLogWarnings = false;

		TestEqual("NumScreens", ScreenManager->NumScreens(), 3);
		TestTrue("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen0));
		TestTrue("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen1));
		TestEqual("GetScreenAtIndex", Screen0, Cast<UScreenHarness>(ScreenManager->GetScreenAtIndex(0)));
		TestEqual("GetScreenAtIndex", Screen1, Cast<UScreenHarness>(ScreenManager->GetScreenAtIndex(1)));
		TestEqual("GetScreenAtIndex", Screen0, Cast<UScreenHarness>(ScreenManager->GetScreenAtIndex(2)));
		TestEqual("GetTop", ScreenManager->GetScreenOnTop(), Cast<UUserWidget>(Screen0));
	}

	return true;
}