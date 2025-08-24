// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "Tooltip.generated.h"

class UViewScreenComponent;
class IEntryWidgetInterface;

/**
 * Listens to provided Screen Components and can display detailed information about a hovered or focused entry
 */
UCLASS()
class UIPOWERTOOLS_API UTooltip : public UCommonActivatableWidget, public IUICSAccessor
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	// get all view selectors
	UFUNCTION(BlueprintCallable)
	const TArray<FViewComponentSelector>& GetAllViewSelectors() const {return ViewsToListenTo;}

	// populate the tooltip with the given data
	UFUNCTION(BlueprintCallable)
	void Populate(UObject* Data);

	// reset the tooltip
	UFUNCTION(BlueprintCallable)
	void Reset();

protected:

	// Populate the widget. The given EntryData can also be retrieved from ObservedEntryData
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Populate"))
	void BP_Populate(UObject* EntryData);

	// Reset the tooltip
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Reset"))
	void BP_Reset();

	// list out BP Events that can happen to an entry widget that is being observed  
	
	// handle the case where the observed entry has an input action happen
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="HandleOnAction"))
	void BP_HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget);

	// handle the case where the observed entry has it's selection property changed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="HandleOnSelectionChange"))
	void BP_HandleOnSelectionChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// handle the case where the observed entry has it's focus property changed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="HandleOnFocusChange"))
	void BP_HandleOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// handle the case where the observed entry has it's hover property changed
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="HandleOnHoverChange"))
	void BP_HandleOnHoverChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	UFUNCTION()
	virtual void HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget);
	UFUNCTION()
	virtual void HandleOnSelectionChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);
	UFUNCTION()
	virtual void HandleOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);
	UFUNCTION()
	virtual void HandleOnHoverChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	virtual void StartListeningToView(const FViewComponentSelector& View);
	virtual void StopListeningToView(const FViewComponentSelector& View);

	virtual void ShowOrHideTooltip(const TScriptInterface<IEntryWidgetInterface>& Widget, const bool bGained);
	virtual void SetObservedEntryData(UObject* InData);

	// the view screen components that we will listen to events from
	UPROPERTY(EditAnywhere)
	TArray<FViewComponentSelector> ViewsToListenTo;

	// show tooltip when gaining hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowOnFocusGain = true;

	// hide tooltip when losing hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHideOnFocusLoss = true;

	// show tooltip when gaining hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowOnHoverGain = true;

	// hide tooltip when losing hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHideOnHoverLoss = true;


	// visibility rule to use when showing tooltip
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility ShowRule = ESlateVisibility::SelfHitTestInvisible;

	// visibility rule to use when hiding tooltip
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility HideRule = ESlateVisibility::Hidden;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> ObservedEntryData;
};
