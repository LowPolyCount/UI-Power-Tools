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
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewActionComp OnAction;
	// a widget has gained or lost selection
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewEventComp OnSelectionChange;
	// a widget has gained or lost focus
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewEventComp OnFocusChange;

	// a widget has gained or lost focus
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewEventComp OnHoverChange;

	// UUserWidget
	virtual void Initialize() override;
	virtual void NativePreConstruct(bool bIsDesignTime) override;
	virtual void NativeDestruct() override;
	// End UUserWidget



	// set the panel widget that our created widgets will attach to.
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetPanel(UPanelWidget* InPanel) { Panel = InPanel; }

	// get the panel widget that created widgets are attached to.
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	UPanelWidget* GetPanel() const { return Panel; }

	// set a data component that we listen to and create widgets to display from
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetLinkedDataComponent(UDataScreenComponent* InData);

	// Get data component we're listening to data for 
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	UDataScreenComponent* GetLinkedDataComponent() const { return LinkedDataComponent; }

	// set the widget prototype used by passing in an instance 
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetWidgetPrototype(UUserWidget* WidgetPrototype);

	// Set the widget prototype used by passing in a class instead of an instance
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetWidgetPrototypeByClass(UClass* WidgetPrototypeClass);

	// get the widget prototype used 
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	UUserWidget* GetWidgetPrototype() const {return EntryWidgetPrototype;}

	// widgets
	// get all view widgets being used
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	const TArray<TScriptInterface<IEntryWidgetInterface>>& GetAllViewWidgets() const { return ActiveViewWidgets; }

	// get the view widget at index
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TScriptInterface<IEntryWidgetInterface> GetViewWidgetAt(int32 Index) const;

	// do we have at least one selected widget?
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	bool IsSelectedWidget() const;

	// will the first widget in this component be the initial focus target?
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	bool IsDesiredFocusTarget() const { return InitialFocus; }

	// set if the first widget in this component will be the initial focus target. Must be set before NativeConstruct is called.
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetIsDesiredFocusTarget(bool bInIsFocusTarget) { InitialFocus = bInIsFocusTarget; }

	// get the first selected widget
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TScriptInterface<IEntryWidgetInterface> GetFirstSelectedWidget() const;

	// get all selected widgets
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TArray<TScriptInterface<IEntryWidgetInterface>> GetAllSelectedWidgets() const;

	// options
	// set if only one widget can be selected at a time
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetSingleSelection(bool bInSingleSelection) { bSingleSelection = bInSingleSelection;}

	// get if only one widget can be selected at a time
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	bool GetSingleSelection() const { return bSingleSelection; }

	// for if you want to give data to the view instead of through a data component
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void ManuallySetData(const TArray<UObject*>& Entries);

protected:
	// a widget has executed an input action (such as being clicked)
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnAction(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget);

	// the selected widget has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnSelectedChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnHoverChange(UViewScreenComponent* Component, const TScriptInterface<IEntryWidgetInterface>& Widget, bool bGained);

	// delegate functions
	UFUNCTION()
	virtual void HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries);
	UFUNCTION()
	void HandleWidgetOnAction(TScriptInterface<IEntryWidgetInterface> Widget);
	UFUNCTION()
	void HandleWidgetOnSelectionChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnFocusChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnHoverChange(TScriptInterface<IEntryWidgetInterface> Widget, bool bGained);

	TScriptInterface<IEntryWidgetInterface> GetAndSetupEntryWidget();

	void AddToPanel(TScriptInterface<IEntryWidgetInterface>& Widget);

	void RemoveEntryWidget(TScriptInterface<IEntryWidgetInterface> Widget);
	void ListenToWidgetDelegates(TScriptInterface<IEntryWidgetInterface> Widget);
	void RemoveWidgetDelegates(TScriptInterface<IEntryWidgetInterface> Widget);

	// data screen component we expect to receive data from
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	FDataComponentSelector DataToListenTo;

	// used in the editor to pick the panel that you want your widgets attached to. 
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	FWidgetSelector PanelSelector;

	// number of entries to show in design view
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent, Meta=(UIMin=0))
	int32 DesignEntriesToShow = 3;

	// will the first widget in this component be the initial focus target?
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	bool InitialFocus = true;

	// are widgets cached when taken off the panel?
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	bool bCacheWidgets = true;


	// can only one widget can be selected at a time?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ViewScreenComponent)
	bool bSingleSelection = false;
	
	// define the class used to display data.  This is done using the prototype pattern, so clones of this instance are made which lets you set
	// specific values in the editor for this class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ViewScreenComponent, Meta=(MustImplement="/Script/UICS.EntryWidgetInterface"))
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