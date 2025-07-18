// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/Tools/WidgetSelector.h"
#include "ComponentSelector.generated.h"

class UScreenComponent;

// holds a component that was selected by the user and can be accessed at runtime
USTRUCT(BlueprintType)
struct UIPOWERTOOLS_API FComponentSelector : public FSelector
{
	GENERATED_BODY()
public:
	FComponentSelector();
#ifdef WITH_EDITORONLY_DATA
	FComponentSelector(const UClass* Filter);
#endif
	virtual bool IsValid() const override { return ComponentGuid.IsValid(); }

	const FGuid& GetComponentGuid() const { return ComponentGuid; }
	void SetComponent(const UScreenComponent* InComponent);

public:
	// needs to be marked as EditAnywhere so that it can be saved by the customizer class
	UPROPERTY(EditAnywhere, Category = ComponentSelector)
	FGuid ComponentGuid = FGuid();
};


// holds a component that was selected by the user and can be accessed at runtime
USTRUCT()
struct UIPOWERTOOLS_API FDataComponentSelector : public FComponentSelector
{
	GENERATED_BODY()
public:
	FDataComponentSelector();
};

// holds a component that was selected by the user and can be accessed at runtime
USTRUCT()
struct UIPOWERTOOLS_API FViewComponentSelector : public FComponentSelector
{
	GENERATED_BODY()
public:
	FViewComponentSelector();
};

// holds a component that was selected by the user and can be accessed at runtime
USTRUCT()
struct UIPOWERTOOLS_API FTransactionComponentSelector : public FComponentSelector
{
	GENERATED_BODY()
public:
	FTransactionComponentSelector();
};

// holds a component that was selected by the user and can be accessed at runtime
USTRUCT()
struct UIPOWERTOOLS_API FEntryComponentSelector : public FComponentSelector
{
	GENERATED_BODY()
public:
	FEntryComponentSelector();
};
