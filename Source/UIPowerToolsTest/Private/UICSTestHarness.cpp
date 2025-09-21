// Copyright 2025 Joel Gonzales

#include "UICSTestHarness.h"
#include "Algo/Reverse.h"

void UDataRetrieverHarness::RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	// for this test, we're making entries that hold only a basic uobject
	for (int32 i = 0; i < NumTestEntries; ++i)
	{
		UObjectIntHarness* Obj = NewObject<UObjectIntHarness>(this);
		Obj->Num = i;
		RetrievedEntries.Emplace(Obj);
	}
}

void UDataHarness::Initialize()
{
	Super::Initialize();

	OnDataRetrieval.AddDynamic(this, &UDataHarness::HandleOnRetrieval);
}

void UDataHarness::HandleOnRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries)
{
	CountOnRetrieval++;
}

void UDataTransformReverseHarness::TransformEntries_Implementation(TArray<UObject*>& RetrievedEntries)
{
	RetrievedEntries;
	Algo::Reverse(RetrievedEntries);
}

void UViewHarness::Initialize()
{
	Super::Initialize();

	OnAction.AddDynamic(this, &UViewHarness::HandleTestOnAction);
	OnSelectionChange.AddDynamic(this, &UViewHarness::HandleTestOnSelectedChange);
	OnFocusChange.AddDynamic(this, &UViewHarness::HandleTestOnFocusChange);
	OnWidgetsPopulated.AddDynamic(this, &UViewHarness::HandleTestOnWidgetsPopulated);

}

int32 UViewHarness::GetNumCachedWidgets() const
{ 
	int32 RetVal = CachedWidgets.Num();
	return RetVal;
}

void UViewHarness::SetCacheWidgets(bool bInCacheWidgets) 
{ 
	bCacheWidgets = bInCacheWidgets; 
	if (!bCacheWidgets)
	{
		CachedWidgets.Empty();
	}
}

void UViewHarness::HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries)
{
	Super::HandleOnDataRetrieval(Component, Entries);
	CountOnRetrieval++;
}

void UViewHarness::HandleTestOnAction(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget)
{
	CountOnAction++;
}

void UViewHarness::HandleTestOnSelectedChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained)
{
	CountOnSelected++;
	(bGained) ? CountOnSelectionGained++ : CountOnSelectionLost++;
}

void UViewHarness::HandleTestOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained)
{
	CountOnFocusChanged++;
	(bGained) ? CountOnFocusGained++ : CountOnFocusLost++;
}

void UViewHarness::HandleTestOnWidgetsPopulated(UViewScreenComponent* Component)
{
	CountOnWidgetsPopulated++;
}

void UTransactionHarness::Initialize()
{
	Super::Initialize();
	OnIsValidResult.AddDynamic(this, &UTransactionHarness::HandleOnIsValid);
	OnExecuteResult.AddDynamic(this, &UTransactionHarness::HandleOnComplete);
	//AddTransaction();
	//AddTransaction(TEST_FAIL, FTransactionInfo(OnFail, OnFail));
	//AddTransaction(TEST_SUCCESS, FTransactionInfo(OnSuccess, OnSuccess));
}

void UTransactionHarness::HandleOnIsValid(UActionScreenComponent* Component, bool bIsValid)
{
	if (bIsValid)
	{
		IsValidSuccess++;
	}
	
}
void UTransactionHarness::HandleOnComplete(UActionScreenComponent* Component, ETransactionResult Result)
{
	if(Result == ETransactionResult::Success)
	{ 
		OnCompleteSuccess++;
	}
}