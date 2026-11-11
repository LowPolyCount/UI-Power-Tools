// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetSelector.generated.h"

class UWidget;

// holds information needed to resolve a selected widget.  
USTRUCT(BlueprintType)
struct UICS_API FWidgetResolver
{
	GENERATED_BODY()
public:

};

USTRUCT(BlueprintType)
struct UICS_API FSelector
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
struct UICS_API FWidgetSelector : public FSelector
{
	GENERATED_BODY()
public:
	virtual bool IsValid() const override;

	FWidgetSelector() = default;
	virtual ~FWidgetSelector(){}
	FWidgetSelector(const UClass* Filter);

	UWidget* GetSelectedWidget(UWidget* WidgetOwner) const;
	FName GetSelectedWidgetName() const {return WidgetName;}
	const TArray<FName>& GetSelectedWidgetPath() const {return WidgetPath;}

#if WITH_EDITORONLY_DATA
	UWidget* GetWidgetAtEditorTime(UWidget* WidgetOwner) const { return GetSelectedWidgetInternal(WidgetOwner, true); }
#endif
 
	//@note: We save these values in our customizer which requires them to be public and set to EditAnywhere, BlueprintReadWrite
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WidgetSelector)
	FName WidgetName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WidgetSelector)
	TArray<FName> WidgetPath;

protected:
	// bInBlueprint is for cases where we need to get the selected widget while editing a blueprint. 
	// How we retrieve the widget tree is different in that case
	UWidget* GetSelectedWidgetInternal(UWidget* WidgetOwner, bool bInBlueprint) const;
};

// Select only Panel Widgets or derived
// @note: Widget must return CanAddMoreChildren() with true to be selectable
USTRUCT()
struct UICS_API FPanelWidgetSelector : public FWidgetSelector
{
	GENERATED_BODY()
	FPanelWidgetSelector();
};