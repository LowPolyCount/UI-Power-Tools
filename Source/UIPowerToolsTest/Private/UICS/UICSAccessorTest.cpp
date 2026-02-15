// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestHelpers.h"
#include "UICSTestHarness.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "UI/Screens/UICS/DataScreenComponent.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Utility/UIPTStatics.h"
#include "UI/Screens/UICSScreen.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAccessorTest, "UIPowerTools.UICS.Accessor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FAccessorTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	UDataHarness* Data = NewObject<UDataHarness>(Screen);
	UDataHarness* Data2 = NewObject<UDataHarness>(Screen);
	UDataHarness* Data3 = NewObject<UDataHarness>(Screen);
	UViewHarness* View = NewObject<UViewHarness>(Screen);

	TestNotNull("Data", Data);
	TestNotNull("Data2", Data2);
	TestNotNull("Data3", Data3);
	TestNotNull("View", View);

	const FName Data2Name = FName(TEXT("Data2TestName"));
	const FName ViewTestName = FName(TEXT("ViewTestName"));

	Data2->SetFName(Data2Name);
	View->SetFName(ViewTestName);

	Screen->AddComponent(Data);
	Screen->AddComponent(View);
	Screen->AddComponent(Data2);
	Screen->AddComponent(Data3);



	// test our basic get functions from the screen
	// we want to test that these deprecated function still work until they are removed
	PRAGMA_DISABLE_INTERNAL_WARNINGS
	{
		TestTrue("Screen->GetComponent<UUICSData>()", Screen->GetScreenComponent<UDataScreenComponent>() == Data);
		TestTrue("Screen->GetComponent<UUICSView>()", Screen->GetScreenComponent<UViewScreenComponent>() == View);

		// test using GetByName
		TestTrue("Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name)", Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name) == Data2);
		TestTrue("Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName)", Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName) == View);

		TArray<UDataScreenComponent*> AllComponents = Screen->GetAllScreenComponents<UDataScreenComponent>();
		TestTrue("AllComponents.Num()", AllComponents.Num() == 3);
		TestTrue("AllComponents[0]", AllComponents[0] == Data);
		TestTrue("AllComponents[1]", AllComponents[1] == Data2);
		TestTrue("AllComponents[2]", AllComponents[2] == Data3);
	}

	// test getter functions from a component
	{
		TestTrue("Data->GetComponent<UUICSData>()", Data->GetScreenComponent<UDataScreenComponent>() == Data);
		TestTrue("Data->GetComponent<UUICSView>()", Data->GetScreenComponent<UViewScreenComponent>() == View);

		TestTrue("Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name)", Data->GetScreenComponentByName<UDataScreenComponent>(Data2Name) == Data2);
		TestTrue("Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName)", Data->GetScreenComponentByName<UViewScreenComponent>(ViewTestName) == View);

		TArray<UDataScreenComponent*> AllComponents = Data->GetAllScreenComponents<UDataScreenComponent>();
		TestTrue("AllComponents.Num()", AllComponents.Num() == 3);
		TestTrue("AllComponents[0]", AllComponents[0] == Data);
		TestTrue("AllComponents[1]", AllComponents[1] == Data2);
		TestTrue("AllComponents[2]", AllComponents[2] == Data3);
	}

	// test selectors
	{
		FComponentSelector Selector;
		Selector.SetComponent(Data);
		TestTrue("GetComponentFromSelector Failed", Screen->GetScreenComponentFromSelector<UDataScreenComponent>(Selector) == Data);
	}

	// test what happens when a component does not have a screen as an outer
	{
		UViewScreenComponent* ViewWithNoParent = NewObject<UViewScreenComponent>();
		TestNotNull("Data", Data);

		bSuppressLogs = true;

		TestNull("ViewWithNoParent->GetComponent<UUICSView>()", ViewWithNoParent->GetScreenComponent<UViewScreenComponent>());

		bSuppressLogs = false;
	}
	PRAGMA_ENABLE_INTERNAL_WARNINGS

	return true;
}

// run the test again, but with someone other than screen implementing IUICSScreenAccessor
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInterfaceAccessorTest, "UIPowerTools.UICS.InterfaceAccessor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FInterfaceAccessorTest::RunTest(const FString& Parameters)
{
	UUICSTestActivatableWidgetHarness* Screen = NewObject<UUICSTestActivatableWidgetHarness>();
	TestNotNull("Screen", Screen);

	UDataHarness* Data = NewObject<UDataHarness>(Screen);
	UDataHarness* Data2 = NewObject<UDataHarness>(Screen);
	UDataHarness* Data3 = NewObject<UDataHarness>(Screen);
	UViewHarness* View = NewObject<UViewHarness>(Screen);

	TestNotNull("Data", Data);
	TestNotNull("Data2", Data2);
	TestNotNull("Data3", Data3);
	TestNotNull("View", View);

	const FName Data2Name = FName(TEXT("Data2TestName"));
	const FName ViewTestName = FName(TEXT("ViewTestName"));

	Data2->SetFName(Data2Name);
	View->SetFName(ViewTestName);

	Screen->AddComponent(Data);
	Screen->AddComponent(View);
	Screen->AddComponent(Data2);
	Screen->AddComponent(Data3);

	// we want to test that these deprecated function still work until they are removed
	PRAGMA_DISABLE_INTERNAL_WARNINGS

	// test our basic get functions from the screen
	{
		TestTrue("Screen->GetComponent<UUICSData>()", Screen->GetScreenComponent<UDataScreenComponent>() == Data);
		TestTrue("Screen->GetComponent<UUICSView>()", Screen->GetScreenComponent<UViewScreenComponent>() == View);

		// test using GetByName
		TestTrue("Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name)", Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name) == Data2);
		TestTrue("Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName)", Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName) == View);

		TArray<UDataScreenComponent*> AllComponents = Screen->GetAllScreenComponents<UDataScreenComponent>();
		TestTrue("AllComponents.Num()", AllComponents.Num() == 3);
		TestTrue("AllComponents[0]", AllComponents[0] == Data);
		TestTrue("AllComponents[1]", AllComponents[1] == Data2);
		TestTrue("AllComponents[2]", AllComponents[2] == Data3);
	}

	// test getter functions from a component
	{
		TestTrue("Data->GetComponent<UUICSData>()", Data->GetScreenComponent<UDataScreenComponent>() == Data);
		TestTrue("Data->GetComponent<UUICSView>()", Data->GetScreenComponent<UViewScreenComponent>() == View);

		// test using GetByName
		TestTrue("Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name)", Data->GetScreenComponentByName<UDataScreenComponent>(Data2Name) == Data2);
		TestTrue("Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName)", Data->GetScreenComponentByName<UViewScreenComponent>(ViewTestName) == View);

		TArray<UDataScreenComponent*> AllComponents = Data->GetAllScreenComponents<UDataScreenComponent>();
		TestTrue("AllComponents.Num()", AllComponents.Num() == 3);
		TestTrue("AllComponents[0]", AllComponents[0] == Data);
		TestTrue("AllComponents[1]", AllComponents[1] == Data2);
		TestTrue("AllComponents[2]", AllComponents[2] == Data3);
	}

	// test selectors
	{
		FComponentSelector Selector;
		Selector.SetComponent(Data);
		TestTrue("GetComponentFromSelector Failed", Screen->GetScreenComponentFromSelector<UDataScreenComponent>(Selector) == Data);
	}

	// test what happens when a component does not have a screen as an outer
	{
		UViewScreenComponent* ViewWithNoParent = NewObject<UViewScreenComponent>();
		TestNotNull("Data", Data);

		bSuppressLogs = true;

		TestNull("ViewWithNoParent->GetComponent<UUICSView>()", ViewWithNoParent->GetScreenComponent<UViewScreenComponent>());

		bSuppressLogs = false;
	}
	PRAGMA_ENABLE_INTERNAL_WARNINGS

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStaticAccessorTest, "UIPowerTools.UICS.Static-Accessor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FStaticAccessorTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	UDataHarness* Data = NewObject<UDataHarness>(Screen);
	UDataHarness* Data2 = NewObject<UDataHarness>(Screen);
	UDataHarness* Data3 = NewObject<UDataHarness>(Screen);
	UViewHarness* View = NewObject<UViewHarness>(Screen);

	TestNotNull("Data", Data);
	TestNotNull("Data2", Data2);
	TestNotNull("Data3", Data3);
	TestNotNull("View", View);

	const FName Data2Name = FName(TEXT("Data2TestName"));
	const FName ViewTestName = FName(TEXT("ViewTestName"));

	Data2->SetFName(Data2Name);
	View->SetFName(ViewTestName);

	Screen->AddComponent(Data);
	Screen->AddComponent(View);
	Screen->AddComponent(Data2);
	Screen->AddComponent(Data3);

	// test our basic get functions from the screen
	{
		TestTrue("Screen->GetComponent<UUICSData>()", UUIPTStatics::GetScreenComponent<UDataScreenComponent>(Screen) == Data);
		TestTrue("Screen->GetComponent<UUICSView>()", UUIPTStatics::GetScreenComponent<UViewScreenComponent>(Screen) == View);

		// test using GetByName
		TestTrue("Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name)", UUIPTStatics::GetScreenComponentByName<UDataScreenComponent>(Screen, Data2Name) == Data2);
		TestTrue("Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName)", UUIPTStatics::GetScreenComponentByName<UViewScreenComponent>(Screen, ViewTestName) == View);

		TArray<UDataScreenComponent*> AllComponents = UUIPTStatics::GetAllScreenComponents<UDataScreenComponent>(Screen);
		TestTrue("AllComponents.Num()", AllComponents.Num() == 3);
		TestTrue("AllComponents[0]", AllComponents[0] == Data);
		TestTrue("AllComponents[1]", AllComponents[1] == Data2);
		TestTrue("AllComponents[2]", AllComponents[2] == Data3);
	}

	// test getter functions from a component
	{
		TestTrue("Data->GetComponent<UUICSData>()", UUIPTStatics::GetScreenComponent<UDataScreenComponent>(Data) == Data);
		TestTrue("Data->GetComponent<UUICSView>()", UUIPTStatics::GetScreenComponent<UViewScreenComponent>(Data) == View);

		TestTrue("Screen->GetScreenComponentByName<UDataScreenComponent>(Data2Name)", UUIPTStatics::GetScreenComponentByName<UDataScreenComponent>(Data, Data2Name) == Data2);
		TestTrue("Screen->GetScreenComponentByName<UViewScreenComponent>(ViewTestName)", UUIPTStatics::
		GetScreenComponentByName<UViewScreenComponent>(Data, ViewTestName) == View);

		TArray<UDataScreenComponent*> AllComponents = UUIPTStatics::GetAllScreenComponents<UDataScreenComponent>(Data);
		TestTrue("AllComponents.Num()", AllComponents.Num() == 3);
		TestTrue("AllComponents[0]", AllComponents[0] == Data);
		TestTrue("AllComponents[1]", AllComponents[1] == Data2);
		TestTrue("AllComponents[2]", AllComponents[2] == Data3);
	}

	// test selectors
	{
		FComponentSelector Selector;
		Selector.SetComponent(Data);
		TestTrue("GetComponentFromSelector Failed", UUIPTStatics::GetScreenComponentFromSelector<UDataScreenComponent>(Data, Selector) == Data);
	}

	// test what happens when a component does not have a screen as an outer
	{
		UViewScreenComponent* ViewWithNoParent = NewObject<UViewScreenComponent>();
		TestNotNull("Data", Data);

		bSuppressLogs = true;

		TestNull("ViewWithNoParent->GetComponent<UUICSView>()", UUIPTStatics::GetScreenComponent<UViewScreenComponent>(ViewWithNoParent));

		bSuppressLogs = false;
	}

	return true;
}
