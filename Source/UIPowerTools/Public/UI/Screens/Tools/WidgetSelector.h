// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetSelector.generated.h"

class UWidget;

// holds information needed to resolve a selected widget.  
USTRUCT(BlueprintType)
struct UIPOWERTOOLS_API FWidgetResolver
{
	GENERATED_BODY()
public:

};

USTRUCT(BlueprintType)
struct UIPOWERTOOLS_API FSelector
{
	GENERATED_BODY()
public:
	FSelector() = default;
	FSelector(const UClass* Filter);
	virtual ~FSelector() = default;
	virtual bool IsValid() const { return false; }

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Selector)
	const UClass* ClassFilter = nullptr;
#endif
};

// Allows the user to select a widget in the editor and look it up in runtime. 
USTRUCT(BlueprintType)
struct UIPOWERTOOLS_API FWidgetSelector : public FSelector
{
	GENERATED_BODY()
public:
	virtual bool IsValid() const override;

	FWidgetSelector() = default;
	virtual ~FWidgetSelector(){}
	FWidgetSelector(const UClass* Filter);
	UWidget* GetWidget(UWidget* WidgetOwner) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WidgetSelector)
	FName WidgetName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WidgetSelector)
	TArray<FName> WidgetPath;
};

// Select only Panel Widgets or derived
// @note: Widget must return CanAddMoreChildren() with true to be selectable
USTRUCT()
struct UIPOWERTOOLS_API FPanelWidgetSelector : public FWidgetSelector
{
	GENERATED_BODY()
	FPanelWidgetSelector();
};