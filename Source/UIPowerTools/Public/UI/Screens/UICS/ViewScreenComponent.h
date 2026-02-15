// Copyright 2025 Joel Gonzales

#pragma once

#include "Engine/MemberReference.h"
#include "UI/Screens/UICS/ScreenComponent.h"
#include "UI/Screens/UICS/ViewWidgetInterface.h"
#include "UI/Screens/Tools/WidgetSelector.h"
#include "ViewScreenComponent.generated.h"

class UPanelWidget;
class UDataScreenComponent;



// a generic event coming from this component
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewComp, UViewScreenComponent*, Component);
// an event involving a widget
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewActionComp, UViewScreenComponent*, Component, const TScriptInterface<IViewWidgetInterface>&, Widget);
// an event involving a widget where the widget gains or loses something
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FViewEventComp, UViewScreenComponent*, Component, const TScriptInterface<IViewWidgetInterface>&, Widget, bool, bGained);



// a struct that holds pointers to a UserWidget that implements IViewWidgetInterface and the underlying SWidget
// this makes it easy for us to hold on to the underlying SWidget so that it isn't destroyed when the UserWidget leaves the viewport
USTRUCT()
struct FCachedWidget
{
	GENERATED_BODY()
	FCachedWidget() = default;
	FCachedWidget(const TScriptInterface<IViewWidgetInterface>& InWidget);

	UPROPERTY()
	TScriptInterface<IViewWidgetInterface> UserWidget;
	TSharedPtr<SWidget> SlateWidget;
};

// define all bindable events in a struct so that in editor, it will be it's own category
USTRUCT()
struct FBindableViewActions
{
	GENERATED_BODY()

	// an input action has occurred on a widget
	UPROPERTY(EditAnywhere, Category = "Events", Meta = (FunctionReference, AllowFunctionLibraries, PrototypeFunction = "/Script/UIPowerTools.ViewScreenComponent.HandleOnInputAction", DefaultBindingName = "InputAction", DisplayName="OnInputAction"))
	FMemberReference Bind_InputAction;

	// a widget has gained or lost selection
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.HandleWidgetOnSelectionChange", DefaultBindingName="SelectionChange", DisplayName = "OnSelectionChange"))
	FMemberReference  Bind_SelectionChange;

	// a widget has gained or lost focus
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.HandleOnFocusChange", DefaultBindingName="FocusChange", DisplayName = "OnFocusChange"))
	FMemberReference  Bind_FocusChange;

	// a widget has gained or lost focus
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.HandleOnHoverChange", DefaultBindingName="HoverChange", DisplayName = "OnHoverChange"))
	FMemberReference  Bind_HoverChange;

	// Widgets have been created and populated
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.Prototype_WidgetsPopulated", DefaultBindingName="WidgetsPopulated", DisplayName = "OnWidgetsPopulated"))
	FMemberReference  Bind_WidgetsPopulated;
};



// responsible for managing, caching widgets, sending data to display entries and acting as a central point for Widget actions like Focus Gain/Loss
UCLASS(BlueprintType, Blueprintable)
class UIPOWERTOOLS_API UViewScreenComponent : public UScreenComponent
{
	GENERATED_BODY()

public:
	// an input action has occurred on a widget
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent, meta = (DeprecatedProperty, DeprecationMessage = "OnAction is deprecated. Use OnInputAction instead"))
	FViewActionComp OnAction;

	// an input action has occurred on a widget
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewActionComp OnInputAction;
	// a widget has gained or lost selection
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewEventComp OnSelectionChange;
	// a widget has gained or lost focus
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewEventComp OnFocusChange;

	// a widget has gained or lost focus
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewEventComp OnHoverChange;

	// Widgets have been created and populated
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent)
	FViewComp OnWidgetsPopulated;

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
	UUserWidget* GetWidgetPrototype() const {return ViewWidgetPrototype;}



	// widgets

	// get all widgets being used 
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TArray<UUserWidget*> GetAllWidgets() const;

	// get the view widget at index
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	UUserWidget* GetWidgetAt(int32 Index) const;

	// get all view widgets being used
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	const TArray<TScriptInterface<IViewWidgetInterface>>& GetAllViewWidgets() const { return ActiveViewWidgets; }

	// get the view widget at index
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TScriptInterface<IViewWidgetInterface> GetViewWidgetAt(int32 Index) const;

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
	TScriptInterface<IViewWidgetInterface> GetFirstSelectedWidget() const;

	// get all selected widgets
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TArray<TScriptInterface<IViewWidgetInterface>> GetAllSelectedWidgets() const;

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

	// Get the first Widget that can be a Desired/Initial Focus Target
	virtual UWidget* GetDesiredFocusTarget() const;

protected:
	// a widget has executed an input action (such as being clicked)
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnInputAction(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget);

	// the selected widget has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnSelectedChange(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnFocusChange(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnHoverChange(UViewScreenComponent* Component, const TScriptInterface<IViewWidgetInterface>& Widget, bool bGained);

	// delegate functions
	UFUNCTION()
	virtual void HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries);

	UFUNCTION()
	void HandleWidgetOnAction(TScriptInterface<IViewWidgetInterface> Widget);
	UFUNCTION()
	void HandleWidgetOnSelectionChange(TScriptInterface<IViewWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnFocusChange(TScriptInterface<IViewWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnHoverChange(TScriptInterface<IViewWidgetInterface> Widget, bool bGained);

	TScriptInterface<IViewWidgetInterface> GetAndSetupEntryWidget();

	void AddToPanel(TScriptInterface<IViewWidgetInterface>& Widget);
	
	void RemoveViewWidget(TScriptInterface<IViewWidgetInterface> Widget);
	void ListenToWidgetDelegates(TScriptInterface<IViewWidgetInterface> Widget);
	void RemoveWidgetDelegates(TScriptInterface<IViewWidgetInterface> Widget);
	void SetupPreConstructWidgets();
	TScriptInterface<IViewWidgetInterface> DuplicateWidget(const TObjectPtr<UUserWidget>& Prototype);
	virtual void PopulateWidgets(const TArray<UObject*>& Entries);

	// The Data Screen Component we will receive data from
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	FDataComponentSelector DataToListenTo;

	// used in the editor to pick the panel that you want your widgets attached to. 
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	FWidgetSelector PanelSelector;

	// define an instance / prototype of a widget class that implements IViewWidgetInterface that we will use to display our data with.
	// This uses the prototype pattern, meaning that we will close this widget instance when we need to make widgets instead of Creating it from a class.
	// This allows you set properties on this widget through the editor
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite, Category = ViewScreenComponent, Meta=(ObjectMustImplement = "/Script/UIPowerTools.ViewWidgetInterface"))
	TObjectPtr<UUserWidget> ViewWidgetPrototype;

	// number of entries to show in design view
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent, Meta=(UIMin=0))
	int32 DesignEntriesToShow = 3;

	// will the first widget in this component be the initial focus target?
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	bool InitialFocus = true;

	// are widgets cached when removed from their panel? Will Cache both the Widget and Slate Widget
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	bool bCacheWidgets = true;

	// can only one widget can be selected at a time?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ViewScreenComponent)
	bool bSingleSelection = false;

	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Prototype_WidgetsPopulated(UViewScreenComponent* Component) {}
#endif
	// events that the user can bind to in editor
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent, Meta=(DisplayName="Events"));
	FBindableViewActions BindableEvents;


	UPROPERTY(Transient)
	TObjectPtr<UPanelWidget> Panel;
	UPROPERTY(Transient)
	TObjectPtr<UDataScreenComponent> LinkedDataComponent;
	UPROPERTY(Transient)
	TArray<FCachedWidget> CachedWidgets;
	UPROPERTY(Transient)
	TArray<TScriptInterface<IViewWidgetInterface>> ActiveViewWidgets;
};