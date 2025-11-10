// Copyright 2025 Joel Gonzales


#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UICSTestHarness.h"
#include "TestHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntryTest, "UIPowerTools.UICS.Entry", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FEntryTest::RunTest(const FString& Parameters)
{
	UEntryHarness* EntryComponent = NewObject<UEntryHarness>();
	EntryComponent->Initialize();

	{
		EntryComponent->SetEntry(nullptr);
		EntryComponent->SetBroadcastOnAnyEntryChange(false);
		EntryComponent->OnBroadcasts = 0;
		UObjectIntHarness* Data1 = NewObject<UObjectIntHarness>();

		EntryComponent->SetEntry(Data1);
		EntryComponent->SetEntry(Data1);
		TestEqual("Normal Broadcasting", EntryComponent->OnBroadcasts, 1);
	}

	{
		EntryComponent->SetEntry(nullptr);
		EntryComponent->SetBroadcastOnAnyEntryChange(true);
		EntryComponent->OnBroadcasts = 0;
		UObjectIntHarness* Data1 = NewObject<UObjectIntHarness>();

		EntryComponent->SetEntry(Data1);
		EntryComponent->SetEntry(Data1);
		TestEqual("Any Change Broadcasting", EntryComponent->OnBroadcasts, 2);
	}

	return true;
}