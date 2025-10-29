// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UI/Screens/UICS/ScreenComponent.h"
#include "DataScreenComponent.generated.h"

class UUIDataProvider;
class UDataTransform;
class UDataFilter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDataLocationFunction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDataRetrieval, UDataScreenComponent*, Component, const TArray<UObject*>&, Entries);

// responsible for managing & caching widgets to display entries
UCLASS(BlueprintType, Blueprintable)
class UIPOWERTOOLS_API UDataScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:
	// call when data retrieval has finished
	UPROPERTY(BlueprintAssignable)
	FDataRetrieval OnDataRetrieval;

	virtual void Initialize() override { Super::Initialize(); }
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FString GetDisplayNameVerbose() const override;

	// clear existing data entries and retrieve new entries
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	const TArray<UObject*>& RetrieveEntries();

	// set retrieve data on construct
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void SetRetrieveOnConstruct(bool bInRetrieve) { bRetrieveOnConstruct = bInRetrieve; }

	// get if we're retrieving data on construct
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	bool GetRetrieveOnConstruct() const { return bRetrieveOnConstruct; }

	// get all retrieved entries
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	const TArray<UObject*>& GetEntries() const { return RetrievedEntries; }

	// how many entries do we have?
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	int32 GetNumEntries() const { return RetrievedEntries.Num(); }

	// do we have an entry at index?
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	bool HasEntryAt(int32 Index) const { return RetrievedEntries.IsValidIndex(Index); }

	// get entry at given index
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	UObject* GetEntryAt(int32 Index) const;

	// clear all retrieved entries
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void EmptyEntries() { RetrievedEntries.Empty(); }

	// Set the data retriever to use with a given instance
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void SetDataRetriever(UUIDataProvider* InData) { DataProvider = InData; }

	// Set the data retriever to use with a given classtype
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void SetDataRetrieverFromClass(TSubclassOf<UUIDataProvider> InClass);
	
	// get data retriever we're using
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	UUIDataProvider* GetDataRetriever() const { return DataProvider; }

	// Do we have a data retriever?
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	bool HasDataRetriever() const { return DataProvider != nullptr; }

	// data filters
	// add a data filter
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void AddDataFilter(UDataFilter* InFilter) { Filters.Add(InFilter); }

	// remove data filter at index
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void RemoveDataFilter(int32 Index);

	// get the filter at index
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	UDataFilter* GetFilterAt(int32 Index);

	// do we have a filter at index?
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	bool HasFilterAt(int32 Index) { return Filters.IsValidIndex(Index); }

	// get all filters
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	const TArray<UDataFilter*>& GetAllFilters() const { return Filters; }

	// data transforms
	// add a data transform
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void AddDataTransform(UDataTransform* InTransform) { Transforms.Add(InTransform); }

	// remove a transform
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	void RemoveDataTransform(int32 Index);

	// get transform at index
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	UDataTransform* GetTransformAt(int32 Index);

	// do we have a transform at index?
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	bool HasTransformAt(int32 Index) { return Transforms.IsValidIndex(Index); }

	// get all transforms
	UFUNCTION(BlueprintCallable, Category = DataScreenComponent)
	const TArray<UDataTransform*>& GetAllTransforms() const { return Transforms; }

protected:
	// class that will retrieve our data
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = DataScreenComponent)
	TObjectPtr<UUIDataProvider> DataProvider;

	// filters to apply to retrieved data
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = DataScreenComponent)
	TArray<UDataFilter*> Filters;

	// transforms to apply to retrieved data
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = DataScreenComponent)
	TArray<UDataTransform*> Transforms;

	// retrieve entries when the screens construct runs?
	UPROPERTY(EditAnywhere, Category = DataScreenComponent)
	bool bRetrieveOnConstruct = true;

	UPROPERTY()
	TArray<UObject*> RetrievedEntries;
};