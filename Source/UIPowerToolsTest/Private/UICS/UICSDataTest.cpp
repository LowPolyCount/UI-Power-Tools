// Copyright (c) Joel Gonzales

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestHelpers.h"
#include "UICSTestHarness.h"
#include "UI/Screens/Screen.h"




UDataHarness* CreateDataHarness(UScreenHarness* Screen)
{
	UDataHarness* RetVal = UICSTest::CreateComponent<UDataHarness>(Screen);
	if (RetVal)
	{
		RetVal->SetDataRetrieverFromClass(TSubclassOf<UUIDataProvider>(UDataRetrieverHarness::StaticClass()));
	}
	return RetVal;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDataTest, "UIPowerTools.UICS.Data", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FDataTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// test retrieve data with invalid location
	{
		// one day AddExpectedMessage will work
		//AddExpectedMessage(TEXT("No Retrieval Set"));

		UDataHarness* Data = CreateDataHarness(Screen);
		TestNotNull("Data", Data);

		bSuppressLogWarnings = true;
		Data->SetDataRetriever(nullptr);
		Data->RetrieveEntries();
		TestTrue("Data->GetEntryAt()", Data->GetEntryAt(0) == nullptr);
		bSuppressLogWarnings = false;
	}

	// test retrieve data
	{
		UDataHarness* Data = CreateDataHarness(Screen);
		TestNotNull("Data", Data);

		Data->RetrieveEntries();

		// did delegate get pinged?
		TestTrue("Data->CountOnRetrieval", Data->CountOnRetrieval == 1);

		
		// as a test, create x entries.
		TestTrue("Data->GetEntryAt()", Data->GetEntryAt(UDataRetrieverHarness::NumTestEntries - 1) != nullptr);

		// test retrieving entry out of range, implies size of data is correct
		bSuppressLogWarnings = true;
		TestFalse("Data->GetEntryAt()", Data->GetEntryAt(UDataRetrieverHarness::NumTestEntries) == nullptr);
		bSuppressLogs = false;	
	}
	

	// test if on native construct flag works
	{
		UDataHarness* Data = CreateDataHarness(Screen);
		TestNotNull("Data", Data);

		Data->SetRetrieveOnConstruct(false);
		Data->NativeConstruct();

		TestTrue("Data->CountOnRetrieval", Data->CountOnRetrieval == 0);

		Data->SetRetrieveOnConstruct(true);
		Data->NativeConstruct();

		TestTrue("Data->CountOnRetrieval", Data->CountOnRetrieval == 1);
	}

	// test clear 
	{
		UDataHarness* Data = CreateDataHarness(Screen);
		TestNotNull("Data", Data);

		Data->NativeConstruct();
		Data->RetrieveEntries();

		Data->NativeDestruct();

		TestTrue("Data", Data->GetEntries().Num() == 0);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDataFilterTest, "UIPowerTools.UICS.Data.Filter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FDataFilterTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// filter out all entries
	{
		UDataHarness* Data = CreateDataHarness(Screen);
		TestNotNull("Data", Data);

		Data->AddDataFilter(NewObject<UDataFilterAllHarness>());

		Data->RetrieveEntries();

		TestTrue("All Filtered out", Data->GetEntries().Num() == 0);

	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDataTransformTest, "UIPowerTools.UICS.Data.Transform", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FDataTransformTest::RunTest(const FString& Parameters)
{
	UScreenHarness* Screen = NewObject<UScreenHarness>();
	TestNotNull("Screen", Screen);

	// transform the order of the entries
	{
		UDataHarness* Data = CreateDataHarness(Screen);
		TestNotNull("Data", Data);
		Data->AddDataTransform(NewObject<UDataTransformReverseHarness>());
		Data->RetrieveEntries();
		
		TestTrue("Num", Data->GetEntries().Num() == UDataRetrieverHarness::NumTestEntries);

		// test that order has been reversed
		for (int32 i = 0; i < UDataRetrieverHarness::NumTestEntries; ++i)
		{
			TestTrue("Order", Cast<UObjectIntHarness>(Data->GetEntryAt(i))->Num == (UDataRetrieverHarness::NumTestEntries - 1 - i));
		}
	}

	return true;
}


