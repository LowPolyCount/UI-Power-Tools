// Copyright 2025 Joel Gonzales

#pragma once

#include "UI/Screens/UICS/ScreenComponent.h"
#include "UI/Screens/UICS/EntryWidgetInterface.h"
#include "UI/Screens/Tools/WidgetSelector.h"
#include "ViewScreenComponent.generated.h"

class UPanelWidget;
class UDataScreenComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewActionComp, UViewScreenComponent*, Component, const TScriptInterface<IEntryWidgetInterface>&, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FViewEventComp, UViewScreenComponent*, Component, const TScriptInterface<IEntryWidgetInterface>&, Widget, bool, bGained);



// responsible for managing, caching widgets to display entries
UCLASS(BlueprintType, Blueprintable)
class UIPOWERTOOLS_API UViewScreenComponent : public UScreenComponent
{
	GENERATED_BODY()
public:
	// an input action has occured on a widget
	UPROPERTY(BlueprintAssignable)
	FViewActionComp OnAction;
	// a widget has gained or lost selection
	UPROPERTY(BlueprintAssignable)
	FViewEventComp OnSelectionChange;
	// a widget has gained or lost focus
	UPROPERTY(BlueprintAssignable)
	FViewEventComp OnFocusChange;

	// UUserWidget
	virtual void Initialize() override;
	virtual void NativePreConstruct(bool bIsDesignTime) override;
	virtual void NativeDestruct() override;
	// End UUserWidget



	// set the panel widget that our created widgets will attach to.
	UFUNCTION(BlueprintCallable)
	void SetPanel(UPanelWidget* InPanel) { Panel = InPanel; }

	// get the panel widget that created widgets are attached to.
	UFUNCTION(BlueprintCallable)
	UPanelWidget* GetPanel() const { return Panel; }

	// set a data component that we listen to and create widgets to display from
	UFUNCTION(BlueprintCallable)
	void SetLinkedDataComponent(UDataScreenComponent* InData);

	// Get data component we're listening to data for 
	UFUNCTION(BlueprintCallable)
	UDataScreenComponent* GetLinkedDataComponent() const { return LinkedDataComponent; }

	// set the widget prototype used by passing in an instance 
	UFUNCTION(BlueprintCallable)
	void SetWidgetPrototype(UUserWidget* WidgetPrototype);

	// Set the widget prototype used by passing in a class instead of an instance
	UFUNCTION(BlueprintCallable)
	void SetWidgetPrototypeByClass(UClass* WidgetPrototypeClass);

	// get the widget prototype used 
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetWidgetPrototype() const {return EntryWidgetPrototype;}

	// widgets
	// get all view widgets being used
	UFUNCTION(BlueprintCallable)
	const TArray<TScriptInterface<IEntryWidgetInterface>>& GetAllViewWidgets() const { return ActiveViewWidgets; }

	// get the view widget at index
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IEntryWidgetInterface> GetViewWidgetAt(int32 Index) const;

	// do we have at least one selected widget?
	UFUNCTION(BlueprintCallable)
	bool IsSelectedWidget() const;

	// will the first widget in this component be the initial focus target?
	UFUNCTION(BlueprintCallable)
	bool IsDesiredFocusTarget() const { return InitialFocus; }

	// set if the first widget in this component will be the initial focus target. Must be set before NativeConstruct is called.
	UFUNCTION(BlueprintCallable)
	void SetIsDesiredFocusTarget(bool bInIsFocusTarget) { InitialFocus = bInIsFocusTarget; }

	// get the first selected widget
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IEntryWidgetInterface> GetFirstSelectedWidget() const;

	// get all selected widgets
	UFUNCTION(BlueprintCallable)
	TArray<TScriptInterface<IEntryWidgetInterface>> GetAllSelectedWidgets() const;

	// options
	// set if only one widget can be selected at a time
	UFUNCTION(BlueprintCallable)
	void SetSingleSelection(bool bInSingleSelection) { bSingleSelection = bInSingleSelection;}

	// get if only one widget can be selected at a time
	UFUNCTION(BlueprintCallable)
	bool GetSingleSelection() const { return bSingleSelection; }

	// for if you want to give data to the view instead of through a data component
	UFUNCTION(BlueprintCallable)
	void ManuallySetData(const TArray<UObject*>& Entries);

protected:
	// a widget has executed an input action (such as being clicked)
	UFUNCTION(BlueprintImplementableEvent)
	void HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget);

	// the selected widget has changed
	UFUNCTION(BlueprintImplementableEvent)
	void HandleOnSelectedChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent)
	void HandleOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// delegate functions
	UFUNCTION()
	virtual void HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries);
	UFUNCTION()
	void HandleWidgetOnAction(TScriptInterface<IEntryWidgetInterface> Widget);
	UFUNCTION()
	void HandleWidgetOnFocusChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnSelectionChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained);

	TScriptInterface<IEntryWidgetInterface> GetAndSetupEntryWidget();

	void AddToPanel(TScriptInterface<IEntryWidgetInterface>& Widget);

	void RemoveEntryWidget(TScriptInterface<IEntryWidgetInterface> Widget);
	void ListenToWidgetDelegates(TScriptInterface<IEntryWidgetInterface> Widget);
	void RemoveWidgetDelegates(TScriptInterface<IEntryWidgetInterface> Widget);

	// data screen component we expect to receive data from
	UPROPERTY(EditAnywhere)
	FDataComponentSelector DataToListenTo;

	// used in the editor to pick the panel that you want your widgets attached to. 
	UPROPERTY(EditAnywhere)
	FWidgetSelector PanelSelector;

	// number of entries to show in design view
	UPROPERTY(EditAnywhere, Meta=(UIMin=0))
	int32 DesignEntriesToShow = 3;

	// will the first widget in this component be the initial focus target?
	UPROPERTY(EditAnywhere)
	bool InitialFocus = true;

	// are widgets cached when taken off the panel?
	UPROPERTY(EditAnywhere)
	bool bCacheWidgets = true;


	// can only one widget can be selected at a time?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSingleSelection = false;
	
	// define the class used to display data.  This is done using the prototype pattern, so clones of this instance are made which lets you set
	// specific values in the editor for this class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MustImplement="/Script/UICS.EntryWidgetInterface"))
	TObjectPtr<UUserWidget> EntryWidgetPrototype = nullptr;

	UPROPERTY()
	TObjectPtr<UPanelWidget> Panel;
	UPROPERTY()
	TObjectPtr<UDataScreenComponent> LinkedDataComponent;
	UPROPERTY()
	TArray<TScriptInterface<IEntryWidgetInterface>> CachedWidgets;
	UPROPERTY()
	TArray<TScriptInterface<IEntryWidgetInterface>> ActiveViewWidgets;





};