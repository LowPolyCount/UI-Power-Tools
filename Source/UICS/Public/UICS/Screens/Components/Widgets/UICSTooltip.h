// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UICS/Screens/Tools/ComponentSelector.h"
#include "UICSTooltip.generated.h"

class UDisplayScreenComponent;
class IDisplayWidgetInterface;

/**
 * Listens to provided Screen Components and can display detailed information about a hovered or focused entry
 */
UCLASS(meta=(PrioritizeCategories="Tooltip"))
class UICS_API UUICSTooltip : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	UUICSTooltip();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	// get all linked display component selectors
	UFUNCTION(BlueprintCallable, Category = Tooltip)
	const TArray<FDisplayComponentSelector>& GetAllLinkedDisplays() const {return LinkedDisplayScreenComponents;}

	// populate the tooltip with the data from a widget we are observing.
	// Normally, you do not need to call this. It is handled internally by events from a Display Screen Component we are linked to. 
	UFUNCTION(BlueprintCallable, Category = Tooltip)
	void Populate(const TScriptInterface<IDisplayWidgetInterface> Widget, UObject* Data);

	// reset the tooltip
	UFUNCTION(BlueprintCallable, Category = Tooltip)
	void Reset();
	
	// get the data associated with the Widget we are observing
	UFUNCTION(BlueprintCallable, Category = Tooltip)
	UObject* GetEntryData() {return ObservedEntryData;}

protected:

	// Populate the widget. The given EntryData can also be retrieved from GetEntryData()
	// You can get the Display Screen Component involved by calling Widget->GetOwningDisplayScreenComponent()
	UFUNCTION(BlueprintImplementableEvent, Category = Tooltip, Meta=(DisplayName="Populate"))
	void BP_Populate(const TScriptInterface<IDisplayWidgetInterface>& Widget, UObject* EntryData);

	// Reset the tooltip
	UFUNCTION(BlueprintImplementableEvent, Category = Tooltip, Meta = (DisplayName = "Reset"))
	void BP_Reset();

	// list out BP Events that can happen to an entry widget that is being observed  
	
	// handle the case where the observed entry has an input action happen
	UFUNCTION(BlueprintImplementableEvent, Category = Tooltip, Meta=(DisplayName="HandleOnAction"))
	void BP_HandleOnAction(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget);

	// handle the case where the observed entry has it's selection property changed
	UFUNCTION(BlueprintImplementableEvent, Category = Tooltip, Meta=(DisplayName="HandleOnSelectionChange"))
	void BP_HandleOnSelectionChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	// handle the case where the observed entry has it's focus property changed
	UFUNCTION(BlueprintImplementableEvent, Category = Tooltip, Meta=(DisplayName="HandleOnFocusChange"))
	void BP_HandleOnFocusChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	// handle the case where the observed entry has it's hover property changed
	UFUNCTION(BlueprintImplementableEvent, Category = Tooltip, Meta=(DisplayName="HandleOnHoverChange"))
	void BP_HandleOnHoverChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	UFUNCTION()
	virtual void HandleOnAction(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget);
	UFUNCTION()
	virtual void HandleOnSelectionChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);
	UFUNCTION()
	virtual void HandleOnFocusChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);
	UFUNCTION()
	virtual void HandleOnHoverChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	virtual void StartListeningToView(const FDisplayComponentSelector& View);
	virtual void StopListeningToView(const FDisplayComponentSelector& View);

	void CheckForShowAndHide(const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained, bool bPopulateWhenGained, bool bHideWhenLost);
	virtual void ShowOrHideTooltip(const TScriptInterface<IDisplayWidgetInterface>& Widget, const bool bGained);
	
	void SetObservedEntryData(UObject* InData);

	// the display screen components that we will listen to events from
	UPROPERTY(EditAnywhere, Category = Tooltip)
	TArray<FDisplayComponentSelector> LinkedDisplayScreenComponents;

	// call populate (and show this tooltip) when a widget managed by one of the linked display components gains focus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	bool bShowAndPopulateWhenWidgetGainsFocus = true;

	// call populate (and show this tooltip) when a widget managed by one of the linked display components gains hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	bool bShowAndPopulateWhenWidgetGainsHover = false;
	
	// call populate (and show this tooltip) when a widget managed by one of the linked display components gains selection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	bool bShowAndPopulateWhenWidgetGainsSelected = false;
	
	// hide tooltip when widget loses focus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	bool bHideWhenWidgetLosesFocus = false;

	// hide tooltip when a widget loses hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	bool bHideWhenWidgetLosesHover = false;
	
	// hide tooltip when a widget loses selection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	bool bHideWhenWidgetLosesSelected = false;

	// visibility rule to use when showing tooltip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	ESlateVisibility ShowRule = ESlateVisibility::SelfHitTestInvisible;

	// visibility rule to use when hiding tooltip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tooltip)
	ESlateVisibility HideRule = ESlateVisibility::Hidden;

	// data from the entry we're currently observing
	UPROPERTY(BlueprintReadWrite, Category = Tooltip)
	TObjectPtr<UObject> ObservedEntryData;
};
