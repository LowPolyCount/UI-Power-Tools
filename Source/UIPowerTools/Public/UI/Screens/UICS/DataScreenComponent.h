// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/ScreenComponent.h"
#include "DataScreenComponent.generated.h"

class UDataScreenComponentProvider;
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
	UFUNCTION(BlueprintCallable)
	const TArray<UObject*>& RetrieveEntries();

	// set retrieve data on construct
	UFUNCTION(BlueprintCallable)
	void SetRetrieveOnConstruct(bool bInRetrieve) { bRetrieveOnConstruct = bInRetrieve; }

	// get if we're retrieving data on construct
	UFUNCTION(BlueprintCallable)
	bool GetRetrieveOnConstruct() const { return bRetrieveOnConstruct; }

	// get all retrieved entries
	UFUNCTION(BlueprintCallable)
	const TArray<UObject*>& GetEntries() const { return RetrievedEntries; }

	// how many entries do we have?
	UFUNCTION(BlueprintCallable)
	int32 GetNumEntries() const { return RetrievedEntries.Num(); }

	// do we have an entry at index?
	UFUNCTION(BlueprintCallable)
	bool HasEntryAt(int32 Index) const { return RetrievedEntries.IsValidIndex(Index); }

	// get entry at given index
	UFUNCTION(BlueprintCallable)
	UObject* GetEntryAt(int32 Index) const;

	// clear all retrieved entries
	UFUNCTION(BlueprintCallable)
	void EmptyEntries() { RetrievedEntries.Empty(); }

	// Set the data retriever to use with a given instance
	UFUNCTION(BlueprintCallable)
	void SetDataRetriever(UDataScreenComponentProvider* InData) { DataProvider = InData; }

	// Set the data retriever to use with a given classtype
	UFUNCTION(BlueprintCallable)
	void SetDataRetrieverFromClass(TSubclassOf<UDataScreenComponentProvider> InClass);
	
	// get data retriever we're using
	UFUNCTION(BlueprintCallable)
	UDataScreenComponentProvider* GetDataRetriever() const { return DataProvider; }

	// Do we have a data retriever?
	UFUNCTION(BlueprintCallable)
	bool HasDataRetriever() const { return DataProvider != nullptr; }

	// data filters
	// add a data filter
	UFUNCTION(BlueprintCallable)
	void AddDataFilter(UDataFilter* InFilter) { Filters.Add(InFilter); }

	// remove data filter at index
	UFUNCTION(BlueprintCallable)
	void RemoveDataFilter(int32 Index);

	// get the filter at index
	UFUNCTION(BlueprintCallable)
	UDataFilter* GetFilterAt(int32 Index);

	// do we have a filter at index?
	UFUNCTION(BlueprintCallable)
	bool HasFilterAt(int32 Index) { return Filters.IsValidIndex(Index); }

	// get all filters
	UFUNCTION(BlueprintCallable)
	const TArray<UDataFilter*>& GetAllFilters() const { return Filters; }

	// data transforms
	// add a data transform
	UFUNCTION(BlueprintCallable)
	void AddDataTransform(UDataTransform* InTransform) { Transforms.Add(InTransform); }

	// remove a transform
	UFUNCTION(BlueprintCallable)
	void RemoveDataTransform(int32 Index);

	// get transform at index
	UFUNCTION(BlueprintCallable)
	UDataTransform* GetTransformAt(int32 Index);

	// do we have a transform at index?
	UFUNCTION(BlueprintCallable)
	bool HasTransformAt(int32 Index) { return Transforms.IsValidIndex(Index); }

	// get all transforms
	UFUNCTION(BlueprintCallable)
	const TArray<UDataTransform*>& GetAllTransforms() const { return Transforms; }

protected:
	// class that will retrieve our data
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataScreenComponentProvider> DataProvider;

	// filters to apply to retrieved data
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly)
	TArray<UDataFilter*> Filters;

	// transforms to apply to retrieved data
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly)
	TArray<UDataTransform*> Transforms;

	// retrieve entries when the screens construct runs?
	UPROPERTY(EditAnywhere)
	bool bRetrieveOnConstruct = true;

	UPROPERTY()
	TArray<UObject*> RetrievedEntries;
};