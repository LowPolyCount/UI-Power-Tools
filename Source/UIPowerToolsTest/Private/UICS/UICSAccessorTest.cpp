// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestHelpers.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "UI/Screens/UICS/DataScreenComponent.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Screens/Screen.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAccessorTest, "UIPowerTools.UICS.Accessor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FAccessorTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	UDataScreenComponent* Data = NewObject<UDataScreenComponent>(Screen);
	UDataScreenComponent* Data2 = NewObject<UDataScreenComponent>(Screen);
	UDataScreenComponent* Data3 = NewObject<UDataScreenComponent>(Screen);
	UViewScreenComponent* View = NewObject<UViewScreenComponent>(Screen);

	TestNotNull("Data", Data);
	TestNotNull("Data2", Data2);
	TestNotNull("Data3", Data3);
	TestNotNull("View", View);

	Screen->AddComponent(Data);
	Screen->AddComponent(View);
	Screen->AddComponent(Data2);
	Screen->AddComponent(Data3);

	// test our basic get functions from the screen
	{
		TestTrue("Screen->GetComponent<UUICSData>()", Screen->GetScreenComponent<UDataScreenComponent>() == Data);
		TestTrue("Screen->GetComponent<UUICSView>()", Screen->GetScreenComponent<UViewScreenComponent>() == View);

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

	return true;
}

// run the test again, but with someone other than screen implementing IUICSScreenAccessor
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInterfaceAccessorTest, "UIPowerTools.UICS.InterfaceAccessor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FInterfaceAccessorTest::RunTest(const FString& Parameters)
{
	UUICSTestActivatableWidgetHarness* Screen = NewObject<UUICSTestActivatableWidgetHarness>();
	TestNotNull("Screen", Screen);

	UDataScreenComponent* Data = NewObject<UDataScreenComponent>(Screen);
	UDataScreenComponent* Data2 = NewObject<UDataScreenComponent>(Screen);
	UDataScreenComponent* Data3 = NewObject<UDataScreenComponent>(Screen);
	UViewScreenComponent* View = NewObject<UViewScreenComponent>(Screen);

	TestNotNull("Data", Data);
	TestNotNull("Data2", Data2);
	TestNotNull("Data3", Data3);
	TestNotNull("View", View);

	Screen->AddComponent(Data);
	Screen->AddComponent(View);
	Screen->AddComponent(Data2);
	Screen->AddComponent(Data3);

	// test our basic get functions from the screen
	{
		TestTrue("Screen->GetComponent<UUICSData>()", Screen->GetScreenComponent<UDataScreenComponent>() == Data);
		TestTrue("Screen->GetComponent<UUICSView>()", Screen->GetScreenComponent<UViewScreenComponent>() == View);

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

	return true;
}
