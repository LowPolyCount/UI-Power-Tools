// Copyright (c) Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/DataScreenComponent.h"
#include "UI/Screens/UICS/Data/UIDataProvider.h"
#include "UI/Screens/UICS/Data/DataFilter.h"
#include "UI/Screens/UICS/Data/DataTransform.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"
#include "UI/Screens/UICS/EntryScreenComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
// @todo: including AutomationEditorCommon means that tests only build with edtior. Maybe FbxAutomationCommon can help?
#include "Tests/AutomationEditorCommon.h"
#include "Components/HorizontalBox.h"
#include "UICSTestHarness.generated.h"

class UPanelWidget;

UCLASS(Hidden)
class UObjectIntHarness : public UObject
{
	GENERATED_BODY()
public:
	int32 Num = -1;
};

UCLASS(Hidden)
class UDataRetrieverHarness : public UUIDataProvider
{
	GENERATED_BODY()
public:
	virtual void RetrieveEntries(UDataScreenComponent* Component, TArray<UObject*>& RetrievedEntries) override;

	static const int32 NumTestEntries = 3;
	// in case we want to change the number of entries we create during testing
	int32 CurrentTestNum = NumTestEntries;
};

UCLASS(Hidden)
class UDataFilterAllHarness : public UDataFilter
{
	GENERATED_BODY()
protected:
	virtual bool ApplyFilter_Implementation(const UObject* Entry) override { return false; }
};

UCLASS(Hidden)
class UDataTransformReverseHarness : public UDataTransform
{
	GENERATED_BODY()
protected:
	virtual void TransformEntries(TArray<UObject*>& InRetrievedEntries) override;
};

UCLASS(Hidden)
class UDataHarness : public UDataScreenComponent
{
	GENERATED_BODY()
public:
	virtual void Initialize() override;

	UFUNCTION()
	void HandleOnRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries);

	int32 CountOnRetrieval = 0;
};

UCLASS(Hidden)
class UViewWidgetHarness : public UUserWidget, public IViewWidgetInterface
{
	GENERATED_BODY()
public:
	VIEW_WIDGET_BOILERPLATE()
};

UCLASS(Hidden)
class UViewHarness : public UViewScreenComponent
{
	GENERATED_BODY()
public:
	virtual void Initialize() override;
	void TestHandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries) { HandleOnDataRetrieval(Component, Entries); }
	const TArray<TScriptInterface<IViewWidgetInterface>> GetAllCachedWidgets() { CachedWidgets; }
	int32 GetNumCachedWidgets() const;
	void SetCacheWidgets(bool bInCacheWidgets);
	virtual void HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries) override;
	UFUNCTION()
	void HandleTestOnAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);
	UFUNCTION()
	void HandleTestOnSelectedChange(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained);
	UFUNCTION()
	void HandleTestOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained);
	UFUNCTION()
	void HandleTestOnWidgetsPopulated(UViewScreenComponent* Component);

	int32 CountOnAction = 0;
	int32 CountOnSelected = 0;
	int32 CountOnSelectionGained = 0;
	int32 CountOnSelectionLost = 0;
	int32 CountOnFocusGained = 0;
	int32 CountOnFocusLost = 0;
	int32 CountOnFocusChanged = 0;
	int32 CountOnRetrieval = 0;
	int32 CountOnWidgetsPopulated = 0;
};

UCLASS(Hidden)
class UTransactionTestHarness: public UActionScreenComponentProvider
{
	GENERATED_BODY()
public:
	virtual bool CanExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry) override { return bCanTransact; }
	virtual ETransactionResult ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry) override { return (bCanExecuteAction) ? ETransactionResult::Success : ETransactionResult::Failure; }

	bool bCanTransact = true;
	bool bCanExecuteAction = true;
	int32 CallsToCanTransactCalls = 0;
	int32 CallsToExecuteTransaction = 0;
};

UCLASS(Hidden)
class UActionHarness : public UActionScreenComponent
{
	GENERATED_BODY()
public:
	virtual void Initialize() override;

	int32 IsValidSuccess = 0;
	int32 OnCompleteSuccess = 0;

	UFUNCTION()
	void HandleOnIsValid(UActionScreenComponent* Component, bool bIsValid);
	UFUNCTION()
	void HandleOnComplete(UActionScreenComponent* Component, ETransactionResult Result);
};

UCLASS(Hidden)
class UEntryHarness : public UEntryScreenComponent
{
	GENERATED_BODY()
public:
	virtual void Initialize() override;

	int32 OnBroadcasts = 0;

	UFUNCTION()
	void HandleOnEntryChange(UEntryScreenComponent* Component, UObject* OldData, UObject* NewData);
};

namespace UICSTest
{
	template <typename ComponentType>
	static ComponentType* CreateComponent(UObject* Owner)
	{
		ComponentType* RetVal = NewObject<ComponentType>(Owner);
		if (RetVal)
		{
			RetVal->Initialize();
		}
		return RetVal;
	}

	static TArray<UObject*> GenerateEntries(int32 Num, UObject* Outer)
	{
		TArray<UObject*> RetVal;
		for (int32 i = 0; i < Num; ++i)
		{
			// create an object and use that for the entry so that the Entry's IsValid call still works
			RetVal.Emplace(NewObject<UObjectIntHarness>(Outer));
		}

		return RetVal;
	}

	static UWorld* CreateWorld()
	{
		return FAutomationEditorCommonUtils::CreateNewMap();
	}


	static UPanelWidget* SetupViewTest(UViewScreenComponent* View, UDataScreenComponent* Data)
	{
		UPanelWidget* Panel = NewObject<UHorizontalBox>(View);
		Panel->TakeWidget();
		View->SetPanel(Panel);
		View->SetLinkedDataComponent(Data);
		View->SetWidgetPrototypeByClass(UViewWidgetHarness::StaticClass());
		if (Data)
		{
			Data->SetDataRetrieverFromClass(TSubclassOf<UUIDataProvider>(UDataRetrieverHarness::StaticClass()));
			Data->RetrieveEntries();
		}
		return Panel;
	}
}

/*struct FUICSTestSetup
{
public:
	TStrongObjectPtr<UWorld> World = nullptr;
	TStrongObjectPtr<UScreenHarness> Screen = nullptr;
	TStrongObjectPtr<UViewHarness> View = nullptr;
	TStrongObjectPtr<UDataHarness> Data = nullptr;
	TArray<TStrongObjectPtr<UObject>> EntryData;
};*/