// Copyright (c) Joel Gonzales


#include "UI/Screens/UICS/DataScreenComponent.h"
#include "UI/Screens/UICS/Data/DataScreenComponentProvider.h"
#include "UI/Screens/UICS/Data/DataTransform.h"
#include "UI/Screens/UICS/Data/DataFilter.h"
#include "UIPowerTools.h"

void UDataScreenComponent::NativeConstruct()
{
	Super::NativeConstruct();

	if (bRetrieveOnConstruct)
	{
		RetrieveEntries();
	}
}

void UDataScreenComponent::NativeDestruct()
{
	EmptyEntries();

	Super::NativeDestruct();
}

FString UDataScreenComponent::GetDisplayNameVerbose() const
{
	FString RetVal = Super::GetDisplayNameVerbose();
	RetVal += TEXT(": ");
	RetVal += (DataProvider && DataProvider->GetClass()) ? DataProvider->GetClass()->GetFName().ToString() : TEXT("None");
	return RetVal;
}


void UDataScreenComponent::SetDataRetrieverFromClass(TSubclassOf<UDataScreenComponentProvider> InClass)
{
	if (InClass)
	{
		DataProvider = NewObject<UDataScreenComponentProvider>(this, InClass);
	}
}

const TArray<UObject*>& UDataScreenComponent::RetrieveEntries()
{
	RetrievedEntries.Empty();
	if (DataProvider)
	{
		DataProvider->Setup();
		DataProvider->RetrieveEntries(this, RetrievedEntries);
		DataProvider->Teardown();
	}
	else
	{
		UE_LOG(LogUICS, Warning, TEXT("No DataRetriever set for %s"), *GetName());
	}

	for (UDataFilter* Filter : Filters)
	{
		Filter->Setup();
		RetrievedEntries = Filter->FilterEntries(RetrievedEntries);
		Filter->Teardown();
	}

	for (UDataTransform* Transform : Transforms)
	{
		Transform->Setup();
		Transform->TransformEntries(RetrievedEntries);
		Transform->Teardown();
	}

	OnDataRetrieval.Broadcast(this, RetrievedEntries);

	return RetrievedEntries;
}

UObject* UDataScreenComponent::GetEntryAt(int32 Index) const
{
	UObject* RetVal = nullptr;
	if (HasEntryAt(Index))
	{
		RetVal = RetrievedEntries[Index];
	}
	else
	{
		//@todo: Find way of macroing function without breaking plugin packaging
		UE_LOG(LogUICS, Warning, TEXT("GetEntryAt Invalid Index %i"), Index);
	}

	return RetVal;
}

void UDataScreenComponent::RemoveDataFilter(int32 Index)
{
	if (HasFilterAt(Index))
	{
		Filters.RemoveAt(Index);
	}
	else
	{
		//@todo: Find way of macroing function without breaking plugin packaging 
		UE_LOG(LogUICS, Warning, TEXT("RemoveDataFilter Invalid Index %i"), Index);
	}
}

void UDataScreenComponent::RemoveDataTransform(int32 Index)
{
	if (HasTransformAt(Index))
	{
		Transforms.RemoveAt(Index);
	}
	else
	{
		//@todo: Find way of macroing function without breaking plugin packaging
		UE_LOG(LogUICS, Warning, TEXT("RemoveDataTransform Invalid Index %i"), Index);
	}
}

UDataFilter* UDataScreenComponent::GetFilterAt(int32 Index)
{
	UDataFilter* RetVal = nullptr;
	if (HasFilterAt(Index))
	{
		RetVal = Filters[Index];
	}
	else
	{
		//@todo: Find way of macroing function without breaking plugin packaging
		UE_LOG(LogUICS, Warning, TEXT("GetFilterAt Invalid Index %i"), Index);
	}

	return RetVal;
}

UDataTransform* UDataScreenComponent::GetTransformAt(int32 Index)
{
	UDataTransform* RetVal = nullptr;
	if (HasTransformAt(Index))
	{
		RetVal = Transforms[Index];
	}
	else
	{
		//@todo: Find way of macroing function without breaking plugin packaging
		UE_LOG(LogUICS, Warning, TEXT("GetTransformAt Invalid Index %i"), Index);
	}

	return RetVal;
}