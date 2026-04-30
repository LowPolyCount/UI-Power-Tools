// Copyright 2025 Joel Gonzales

#pragma once

#include "UObject/Interface.h"
#include "ViewWidgetInterface.generated.h"

class UViewScreenComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewAction, TScriptInterface<IViewWidgetInterface>, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewEvent, TScriptInterface<IViewWidgetInterface>, Widget, bool, bGained);


// the purpose of this interface is to get around the issue with UWidget events where we don't know who broadcast a Widget Event. 
// Example: If you're an outside class listening to multiple Button's OnClick events, you don't know which button broadcast OnClick. 
// ListView gets around this because it can create it's own SWidget where Slate will tell it which Widget was Clicked/Selected/Focused/etc. 
// We don't have that luxury because View Screen Component needs to be compatible with all UPanels.
// Widgets are required to implement this to be usable with a view component.  
// Widgets implementing this interface are:
// Cacheable - Instead of being destroyed when no longer used, it may be held for future use. 
// Resetable - The OnReset event is called before we set it's data and when the widget is removed from it's parent, so that if the widget is being cached, you can remove listeners, pointers, etc. 
UINTERFACE(BlueprintType)
class UViewWidgetInterface : public UInterface
{
	GENERATED_BODY()
};


class UIPOWERTOOLS_API IViewWidgetInterface
{
	GENERATED_BODY()

public:
	// these delegates need to be created in the implementing class so that they can be set as blueprint assignable
	//@todo: Do they need to be exposed to the user?  They are mostly just used for the view component
	virtual FViewAction& GetOnAction() = 0;
	virtual FViewEvent& GetOnSelectionChange() = 0;
	virtual FViewEvent& GetOnFocusChange() = 0;
	virtual FViewEvent& GetOnHoverChange() = 0;
	// first list the functions that are user facing. Because this is an interface, all BlueprintCallable functions need to be BlueprintNativeEvent even if we don't want to expose them as such. 
	
protected:
	// Called when our widget has received data and should set it's widgets (Images, textfields, etc) based on the contents of the data. 
	// @EntryData - The data that has been received. Comes in as UObject so that we're compatabile with any type of data coming in. You should type it to what the actual class type is. 
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void Populate(UObject* EntryData);

public:
	// set the widget's entry data.  This is only callable in blueprint for edge cases where you may want to use a ViewWidget by itself and need to set it's data.  
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void SetEntryData(int32 InIndex, UObject* InEntry);

	// get the current entry data
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	UObject* GetEntryData() const;

	// get the widget's index on the panel that it is a child of
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	int32 GetIndex() const;

	// reset the widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void Reset();

	// set a pointer to the view screen component that is managing us. 
	void SetOwningViewScreenComponent(UViewScreenComponent* InOwningComponent);

	// get the view screen component that is managing us
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	UViewScreenComponent* GetOwningViewScreenComponent() const;

	// if our owning view screen component has an action component linked to it, get that. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	UActionScreenComponent* GetLinkedActionScreenComponent() const;

	// do we have a valid ASC? 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	bool HasLinkedActionScreenComponent() const;

	// Will query the Action Screen Component linked to this Widget's owning View Screen Component to see if this Widget's EntryData Can be executed upon. 
	// @return Will return results of Action Screen Component's CanExecuteAction() 
	// @return Will return true if the Owning ASC is invalid
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	bool CanExecuteAction();

protected:
	// list out events that are user facing


	// Widget has just had it's entry data set
	// @index - The index this widget is in the entry set
	// @entry - the entry data
	// @todo - consider deprecating this function. 
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnEntryDataSet(int32 InIndex, const UObject* InEntry);

	// Handle any actions required before we receive a new set of data to represent.
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnReset();

	// event notifying that some type of input (Click, Button Press, etc) has happened on this widget
	UFUNCTION(BlueprintNativeEvent, Category = ViewWidget)
	void OnInputAction();

	// If there is a linked Action Component linked to our widgt's View Component, what was the result of calling Action's CanExecute() function?
	// Example: ViewWidgets are displaying items a player can buy, Result would return if the player had enough money to buy the item this widget represents.
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void HandleOnActionExecuteResult(UActionScreenComponent* Component, const FGameplayTag& Result);

	// these internal functions are broken out and made virtual in case an implementing class needs to override them. 
	virtual UObject* GetEntry_Internal() const { return Entry.Get(); }
	virtual void SetEntry_Internal(UObject* InEntry) { Entry = TStrongObjectPtr<UObject>(InEntry); }
	virtual void SetEntryData_Implementation(int32 InIndex, UObject* InEntry);
	virtual UObject* GetEntryData_Implementation() const;
	virtual int32 GetIndex_Implementation() const { return Index; }
	virtual void Reset_Implementation();
	UViewScreenComponent* GetOwningViewScreenComponent_Implementation() const;

	// these take the existing widget event calls, and translates them to a version where we will know who broadcast them. 
	void SetFocus_Internal(bool bInFocused);
	void SetHovered_Internal(bool bInHovered);
	void SetSelected_Internal(bool bInSelected);
	void SetInputAction_Internal();
	bool HasLinkedActionScreenComponent_Implementation() const;
	UActionScreenComponent* GetLinkedActionScreenComponent_Implementation() const;
	bool CanExecuteAction_Implementation();

	TStrongObjectPtr<UObject> Entry;	// the entry data
	int32 Index = INDEX_NONE;			// what is the index of the widget in the view component array?
	TWeakObjectPtr<UViewScreenComponent> OwningViewScreenComponent; // View Component that is managing this widget

public:
	// list out deprecated functions. 
	// set the focus
	UE_DEPRECATED(Any, "Is Deprecated. Use UWidget:: Instead")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetFocus() is deprecated. Use UWidget::SetuserFocus()"))
	void SetFocus(bool bInFocused);
	// set if we are hovered
	UE_DEPRECATED(Any, "Is Deprecated. Try UUserWidget::OnMouseLeave()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetHovered() is deprecated. Try UUserWidget::OnMouseLeave()"))
	void SetHovered(bool bInHovered);
	// set if we are selected
	UE_DEPRECATED(Any, "Is Deprecated.  Use UCommonButtonBase::GetIsSelected()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetSelected() is deprecated. Use UCommonButtonBase::GetIsSelected()"))
	void SetSelected(bool bInSelected);

	// Force an input action to happen
	UE_DEPRECATED(Any, "Is Deprecated. UCommonButton::ExecuteTriggeredInput()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "ForceInputAction() is deprecated. Use UCommonButton::ExecuteTriggeredInput()"))
	void ForceInputAction();
	
	// are we focused?
	UE_DEPRECATED(Any, "Is Deprecated. Use UWidget::HasAnyUserFocus() instead")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta=(DeprecatedFunction, DeprecationMessage="IsFocused() is deprecated. Use UWidget::HasAnyUserFocus() instead"))
	bool IsFocused() const;


	// are we hovered?
	UE_DEPRECATED(Any, "Is Deprecated. Use UWidget::IsHovered()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "IsHovered() is deprecated. Use UWidget::IsHovered()"))
	bool IsHovered() const;

	// are we selected?
	UE_DEPRECATED(Any, "Is Deprecated. Use UCommonButtonBase::GetIsSelected()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "IsSelected() is deprecated. Use UCommonButtonBase::GetIsSelected()"))
	bool IsSelected() const;

protected:
	// event notifying that the focus has changed
	UE_DEPRECATED(Any, "Is Deprecated. Bind to UCommonButtonBase::OnReceivedFocus or UCommonButtonBase::OnLostFocus ")
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "OnFocusChanged() is deprecated. Bind to UCommonButtonBase::OnReceivedFocus or UCommonButtonBase::OnLostFocus "))
	void OnFocusChanged(bool bFocus);

	// event notifying that the selection of the widget has changed
	UE_DEPRECATED(Any, "Is Deprecated. Bind to UCommonButton::OnSelected/OnDeselected")
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "OnSelectionChanged() is deprecated. Bind to UCommonButton::OnSelected/OnDeselected"))
	void OnSelectionChanged(bool bInSelected);


	// the deprecated implementation of the above functions
	void SetFocus_Implementation(bool bInFocused);
	void SetHovered_Implementation(bool bInHovered);
	void SetSelected_Implementation(bool bInSelected);
	void OnInputAction_Implementation();
	bool IsFocused_Implementation() const;
	bool IsHovered_Implementation() const;
	bool IsSelected_Implementation() const;
	void ForceInputAction_Implementation();

};

// these macros help define boilerplate code for UWidget classes that you want to implement IViewWidgetInterface
// This can be complicated because UUserWidget has different functions to define how it gets focus and hover differently than UButton
// 
// this defines what every implementation needs. 
#define VIEW_BASE_BOILERPLATE() \
public:\
UPROPERTY(BlueprintAssignable, Category = ViewWidget)\
FViewAction OnInputAction;\
UPROPERTY(BlueprintAssignable, Category = ViewWidget)\
FViewEvent OnSelectionChanged;\
UPROPERTY(BlueprintAssignable, Category = ViewWidget)\
FViewEvent OnFocusChanged;\
UPROPERTY(BlueprintAssignable, Category = ViewWidget)\
FViewEvent OnHoverChange;\
virtual FViewAction& GetOnAction() { return OnInputAction; }\
virtual FViewEvent& GetOnSelectionChange() { return OnSelectionChanged; }\
virtual FViewEvent& GetOnFocusChange() { return OnFocusChanged; }\
virtual FViewEvent& GetOnHoverChange() { return OnHoverChange; }\

//
// define boilerplate required for a UUserWidget
// @note: a UserWidget doesn't have Action or Selection built in, so we don't need to define those. 
#define VIEW_USERWIDGET_BOILERPLATE() \
VIEW_BASE_BOILERPLATE() \
virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override\
{\
	const FReply RetVal = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);\
	SetFocus_Internal(true);\
	return RetVal;\
}\
virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override\
{\
	Super::NativeOnFocusLost(InFocusEvent);\
	SetFocus_Internal(false);\
}\
virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override\
{\
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);\
	SetHovered_Internal(true);\
}\
virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override\
{\
	Super::NativeOnMouseLeave(InMouseEvent);\
	SetHovered_Internal(false);\
}\

// this macro defines boilerplate code for a class derived from CommonButtonBase that is required for implementing IViewWidgetInterface

#define VIEW_COMMONBUTTON_BOILERPLATE() \
VIEW_USERWIDGET_BOILERPLATE() \
virtual void NativeOnSelected(bool bBroadcast) override\
{\
	Super::NativeOnSelected(bBroadcast);\
	SetSelected_Internal(true);\
}\
virtual void NativeOnDeselected(bool bBroadcast) override\
{\
	Super::NativeOnDeselected(bBroadcast);\
	SetSelected_Internal(false);\
}\
virtual void NativeOnClicked() override\
{\
	Super::NativeOnClicked();\
	SetInputAction_Internal();\
}\
// NativeOnActionComplete is called when a hold is finished, but iirc, NativeOnClicked will also get called, so there is no need for us to override. 
/*virtual void NativeOnActionComplete() override\
{\
	Super::NativeOnActionComplete();\
	SetInputAction_Internal();\
}\*/

// keep old definitions for backwards compatibility
#define VIEW_WIDGET_BOILERPLATE() VIEW_USERWIDGET_BOILERPLATE()
#define VIEW_BUTTON_AND_WIDGET_BOILERPLATE() VIEW_COMMONBUTTON_BOILERPLATE 