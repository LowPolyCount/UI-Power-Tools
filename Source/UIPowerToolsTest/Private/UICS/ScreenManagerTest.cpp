// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UICSTestHarness.h"
#include "TestHelpers.h"
#include "Components/HorizontalBox.h"
#include "UI/Screens/Screen.h"
#include "UI/Screens/ScreenManager.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "UI/Tools/UIPowerToolsDeveloperSettings.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScreenManagerAddRemoveTest, "UIPowerTools.UICS.ScreenManager.AddRemove", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FScreenManagerAddRemoveTest::RunTest(const FString& Parameters)
{
	UScreenManager* ScreenManager = NewObject<UScreenManager>();
	TestNotNull("ScreenManager", ScreenManager);
	

	// test screen adding functionality like visibility, etc. 
	{
		UScreenHarness* Screen0 = NewObject<UScreenHarness>();
		UScreenHarness* Screen1 = NewObject<UScreenHarness>();
		UScreenHarness* Screen2 = NewObject<UScreenHarness>();
		UScreenHarness* Screen3 = NewObject<UScreenHarness>();
		TSubclassOf<UUserWidget> AsSubclassOf(UScreenHarness::StaticClass());

		TestFalse("IsScreenOfClassOnStack", ScreenManager->IsScreenOfClassOnStack(AsSubclassOf));
		
		ScreenManager->AddScreen(Screen0);
		ScreenManager->AddScreen(Screen1);
		ScreenManager->AddScreen(Screen2);

		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible );
		TestEqual("NumScreens", ScreenManager->NumScreens(), 3);
		TestTrue("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen0));
		TestFalse("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen3));
		TestTrue("IsScreenOfClassOnStack", ScreenManager->IsScreenOfClassOnStack(AsSubclassOf));

		ScreenManager->AddScreen(Screen3, false);

		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen1->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("Visibility", Screen3->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 4);

		
		Screen3->Close();
		
		
		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen1->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 3);

		// close screen out of order
		Screen1->Close();

		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::Hidden);
		TestEqual("Visibility", Screen2->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 2);

		Screen2->Close();

		TestEqual("Visibility", Screen0->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
		TestEqual("NumScreens", ScreenManager->NumScreens(), 1);
	}

	ScreenManager->RemoveFromRoot();

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
		ScreenManager->AddScreen(Screen0);
		ScreenManager->AddScreen(Screen1);
		ScreenManager->AddScreen(Screen0);

		TestEqual("NumScreens", ScreenManager->NumScreens(), 3);
		TestTrue("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen0));
		TestTrue("IsScreenOnStack", ScreenManager->IsScreenOnStack(Screen1));
		TestEqual("GetTop", ScreenManager->GetScreenOnTop(), TScriptInterface<IScreenInterface>(Screen0));
	}

	return true;
}