// Copyright 2025 Joel Gonzales

#pragma once

#include "Engine/MemberReference.h"
#include "UICS/Screens/Components/ScreenComponent.h"
#include "UICS/Screens/Components/Display/DisplayWidgetInterface.h"
#include "UICS/Screens/Tools/WidgetSelector.h"
#include "UICS/Screens/Tools/ComponentSelector.h"
#include "DisplayScreenComponent.generated.h"

class UPanelWidget;
class UDataScreenComponent;


// a generic event coming from this component
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewComp, UDisplayScreenComponent*, Component);
// an event involving a widget
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewActionComp, UDisplayScreenComponent*, Component, const TScriptInterface<IDisplayWidgetInterface>&, Widget);
// an event involving a widget where the widget gains or loses something
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FViewEventComp, UDisplayScreenComponent*, Component, const TScriptInterface<IDisplayWidgetInterface>&, Widget, bool, bGained);



// a struct that holds pointers to a UserWidget that implements IDisplayWidgetInterface and the underlying SWidget
// this makes it easy for us to hold on to the underlying SWidget so that it isn't destroyed when the UserWidget leaves the viewport
USTRUCT()
struct FCachedWidget
{
	GENERATED_BODY()
	FCachedWidget() = default;
	FCachedWidget(const TScriptInterface<IDisplayWidgetInterface>& InWidget);

	UPROPERTY()
	TScriptInterface<IDisplayWidgetInterface> UserWidget;
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
	FMemberReference Bind_SelectionChange;

	// a widget has gained or lost focus
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.HandleOnFocusChange", DefaultBindingName="FocusChange", DisplayName = "OnFocusChange"))
	FMemberReference Bind_FocusChange;

	// a widget has gained or lost focus
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.HandleOnHoverChange", DefaultBindingName="HoverChange", DisplayName = "OnHoverChange"))
	FMemberReference Bind_HoverChange;

	// Widgets have been created and populated
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.Prototype_WidgetsPopulated", DefaultBindingName="WidgetsPopulated", DisplayName = "OnWidgetsPopulated"))
	FMemberReference Bind_WidgetsPopulated;

	// get the display component
	UPROPERTY(EditAnywhere, Category = "Events", Meta=(FunctionReference, AllowFunctionLibraries, PrototypeFunction="/Script/UIPowerTools.ViewScreenComponent.Prototype_Get", DefaultBindingName="GetViewComponent"))
	FMemberReference Get;
};



// responsible for managing, caching widgets, sending data to display entries and acting as a central point for Widget actions like Focus Gain/Loss
UCLASS(BlueprintType, Blueprintable)
class UICS_API UDisplayScreenComponent : public UScreenComponent
{
	GENERATED_BODY()

public:
	// UUserWidget
	virtual void Initialize() override;
	virtual void NativePreConstruct(bool bIsDesignTime) override;
	virtual void NativeDestruct() override;
	// End UUserWidget


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

	// get all view widgets being used
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	const TArray<TScriptInterface<IDisplayWidgetInterface>>& GetAllViewWidgets() const { return ActiveViewWidgets; }

	// get the view widget at index
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	UUserWidget* GetWidgetAt(int32 Index) const;

	// get the view widget at index
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TScriptInterface<IDisplayWidgetInterface> GetViewWidgetAt(int32 Index) const;

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
	TScriptInterface<IDisplayWidgetInterface> GetFirstSelectedWidget() const;

	// get all selected widgets
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	TArray<TScriptInterface<IDisplayWidgetInterface>> GetAllSelectedWidgets() const;

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

	// Set an ASC that is listening to our events
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	void SetLinkedActionScreenComponent(UActionScreenComponent* InASC);

	// Get an ASC that is listening to our events.
	UFUNCTION(BlueprintCallable, Category = ViewScreenComponent)
	UActionScreenComponent* GetLinkedActionComponent() const {return LinkedASC;}

	// Get the first Widget that can be a Desired/Initial Focus Target
	virtual UWidget* GetDesiredFocusTarget() const;

protected:
	// a widget has executed an input action (such as being clicked)
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnInputAction(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget);

	// the selected widget has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnSelectedChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnFocusChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	// the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewScreenComponent)
	void HandleOnHoverChange(UDisplayScreenComponent* Component, const TScriptInterface<IDisplayWidgetInterface>& Widget, bool bGained);

	// delegate functions
	UFUNCTION()
	virtual void HandleOnDataRetrieval(UDataScreenComponent* Component, const TArray<UObject*>& Entries);

	UFUNCTION()
	void HandleWidgetOnAction(TScriptInterface<IDisplayWidgetInterface> Widget);
	UFUNCTION()
	void HandleWidgetOnSelectionChange(TScriptInterface<IDisplayWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnFocusChange(TScriptInterface<IDisplayWidgetInterface> Widget, bool bGained);
	UFUNCTION()
	void HandleWidgetOnHoverChange(TScriptInterface<IDisplayWidgetInterface> Widget, bool bGained);

	TScriptInterface<IDisplayWidgetInterface> GetAndSetupEntryWidget();

	void AddToPanel(TScriptInterface<IDisplayWidgetInterface>& Widget);
	
	void RemoveViewWidget(TScriptInterface<IDisplayWidgetInterface> Widget);
	void ViewWidgetSetup(TScriptInterface<IDisplayWidgetInterface> Widget);
	void ViewWidgetTeardown(TScriptInterface<IDisplayWidgetInterface> Widget);
	void SetupPreConstructWidgets();
	TScriptInterface<IDisplayWidgetInterface> DuplicateWidget(const TObjectPtr<UUserWidget>& Prototype);
	virtual void PopulateWidgets(const TArray<UObject*>& Entries);

	// The Data Screen Component we will receive data from
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	FDataComponentSelector DataToListenTo;

	// used in the editor to pick the panel that you want your widgets attached to. 
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent)
	FWidgetSelector PanelSelector;

	// define an instance / prototype of a widget class that implements IDisplayWidgetInterface that we will use to display our data with.
	// This uses the prototype pattern, meaning that we will close this widget instance when we need to make widgets instead of Creating it from a class.
	// This allows you set properties on this widget through the editor
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite, Category = ViewScreenComponent, Meta=(ObjectMustImplement = "/Script/UIPowerTools.ViewWidgetInterface", DisplayName="View Widget To Use"))
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
	
	// events that the user can bind to in editor
	UPROPERTY(EditAnywhere, Category = ViewScreenComponent, Meta=(DisplayName="Events"));
	FBindableViewActions BindableEvents;

	// Data Component that we are listening to 
	UPROPERTY(Transient)
	TObjectPtr<UDataScreenComponent> LinkedDataComponent;

	// an ASC that is listening to our events
	UPROPERTY(Transient)
	TObjectPtr<UActionScreenComponent> LinkedASC;

	// that panel we are attaching our widgets to.
	UPROPERTY(Transient)
	TObjectPtr<UPanelWidget> Panel;

	// list of widgets that are cached so that their SWidgets are not destroyed and will be reused before new widgets are created.
	UPROPERTY(Transient)
	TArray<FCachedWidget> CachedWidgets;

	// list of active widgets we are managing. 
	UPROPERTY(Transient)
	TArray<TScriptInterface<IDisplayWidgetInterface>> ActiveViewWidgets;


	// BEGIN FMember References that allow you to bind events to functions in editor
#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Prototype_WidgetsPopulated(UDisplayScreenComponent* Component) {}

	UFUNCTION(BlueprintInternalUseOnly)
	void Prototype_Get(UDisplayScreenComponent* DisplayComponent)  {}
#endif

public:
	// start deprecated items

	// an input action has occurred on a widget
	UPROPERTY(BlueprintAssignable, Category = ViewScreenComponent, meta = (DeprecatedProperty, DeprecationMessage = "OnAction is deprecated. Use OnInputAction instead"))
	FViewActionComp OnAction;
};