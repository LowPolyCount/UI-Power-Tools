// Copyright 2025 Joel Gonzales

#include "UICSTestHarness.h"
#include "Algo/Reverse.h"

void UDataRetrieverHarness::NativeRetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries)
{
	// for this test, we're making entries that hold only a basic uobject
	for (int32 i = 0; i < NumTestEntries; ++i)
	{
		UObjectIntHarness* Obj = NewObject<UObjectIntHarness>(this);
		Obj->Num = i;
		RetrievedEntries.Emplace(Obj);
	}
}

void UDataHarness::NativeInitialize()
{
	Super::NativeInitialize();

	OnDataRetrieval.AddDynamic(this, &UDataHarness::HandleOnRetrieval);
}

void UDataHarness::HandleOnRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries)
{
	CountOnRetrieval++;
}

void UDataTransformReverseHarness::NativeTransformEntries(TArray<UObject*>& InRetrievedEntries)
{
	Algo::Reverse(InRetrievedEntries);
}

void UViewHarness::NativeInitialize()
{
	Super::NativeInitialize();

	OnInputAction.AddDynamic(this, &UViewHarness::HandleTestOnAction);
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

void UViewHarness::HandleTestOnAction(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget)
{
	CountOnAction++;
}

void UViewHarness::HandleTestOnSelectedChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	CountOnSelected++;
	(bGained) ? CountOnSelectionGained++ : CountOnSelectionLost++;
}

void UViewHarness::HandleTestOnFocusChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained)
{
	CountOnFocusChanged++;
	(bGained) ? CountOnFocusGained++ : CountOnFocusLost++;
}

void UViewHarness::HandleTestOnWidgetsPopulated(UDisplayScreenComponent* Component)
{
	CountOnWidgetsPopulated++;
}

void UActionHarness::NativeInitialize()
{
	Super::NativeInitialize();
	PRAGMA_DISABLE_INTERNAL_WARNINGS
	OnIsValidResult_DEPRECATED.AddDynamic(this, &UActionHarness::HandleOnIsValid);
	PRAGMA_ENABLE_INTERNAL_WARNINGS
	OnActionExecuteResult.AddDynamic(this, &UActionHarness::HandleOnComplete);
}

void UActionHarness::HandleOnIsValid(UActionScreenComponent* Component, bool bIsValid)
{
	if (bIsValid)
	{
		IsValidSuccess++;
	}
	
}
void UActionHarness::HandleOnComplete(UActionScreenComponent* Component, bool bExecuteResult, const FGameplayTag& Result)
{
	if(bExecuteResult == true || Result == UICS_Action_Success)
	{ 
		OnCompleteSuccess++;
	}
}

void UEntryHarness::NativeInitialize()
{
	Super::NativeInitialize();
	OnEntryChange.AddDynamic(this, &UEntryHarness::HandleOnEntryChange);
}

void UEntryHarness::HandleOnEntryChange(UEntryScreenComponent* Component, UObject* OldData, UObject* NewData)
{
	OnBroadcasts++;
}