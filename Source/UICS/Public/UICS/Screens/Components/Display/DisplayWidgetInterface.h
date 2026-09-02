// Copyright 2025 Joel Gonzales

#pragma once

#include "UObject/Interface.h"
#include "GameplayTagsClasses.h"
#include "DisplayWidgetInterface.generated.h"

class UDisplayScreenComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewAction, TScriptInterface<IDisplayWidgetInterface>, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewEvent, TScriptInterface<IDisplayWidgetInterface>, Widget, bool, bGained);


// the purpose of this interface is to get around the issue with UWidget events where we don't know who broadcast a Widget Event. 
// Example: If you're an outside class listening to multiple Button's OnClick events, you don't know which button broadcast OnClick. 
// ListView gets around this because it can create it's own SWidget where Slate will tell it which Widget was Clicked/Selected/Focused/etc. 
// We don't have that luxury because Display Screen Component needs to be compatible with all UPanels.
// Widgets are required to implement this to be usable with a Display component.  
// Widgets implementing this interface are:
// Cacheable - Instead of being destroyed when no longer used, it may be held for future use. 
// Resetable - The OnReset event is called before we set it's data and when the widget is removed from it's parent, so that if the widget is being cached, you can remove listeners, pointers, etc. 
UINTERFACE(BlueprintType)
class UDisplayWidgetInterface : public UInterface
{
	GENERATED_BODY()
};


class UICS_API IDisplayWidgetInterface
{
	GENERATED_BODY()

public:
	// these delegates need to be created in the implementing class so that they can be set as blueprint assignable
	//@todo: Do they need to be exposed to the user?  They are mostly just used for the Display component
	virtual FViewAction& GetOnAction() = 0;
	virtual FViewEvent& GetOnSelectionChange() = 0;
	virtual FViewEvent& GetOnFocusChange() = 0;
	virtual FViewEvent& GetOnHoverChange() = 0;
	// first list the functions that are user facing. Because this is an interface, all BlueprintCallable functions need to be BlueprintNativeEvent even if we don't want to expose them as such. 
	
protected:
	// Called when our widget has received data and should set it's widgets (Images, textfields, etc) based on the contents of the data. 
	// @EntryData - The data that has been received. Comes in as UObject so that we're compatabile with any type of data coming in. You should type it to what the actual class type is. 
	UFUNCTION(BlueprintImplementableEvent, Category = DisplayWidget)
	void Populate(UObject* EntryData);

public:
	// set the widget's entry data.  This is only callable in blueprint for edge cases where you may want to use a DisplayWidget by itself and need to set it's data.  
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	void SetEntryData(int32 InIndex, UObject* InEntry);

	// get the current entry data
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	UObject* GetEntryData() const;

	// get the widget's index on the panel that it is a child of
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	int32 GetIndex() const;

	// reset the widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	void Reset();

	// set a pointer to the display screen component that created us 
	void SetOwningDisplayScreenComponent(UDisplayScreenComponent* InOwningComponent);

	// get the display screen component that created us
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	UDisplayScreenComponent* GetOwningDisplayScreenComponent() const;

	// get the Action Screen Component that is linked to the View Screen Component that created us
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	UActionScreenComponent* GetLinkedActionScreenComponent() const;

	// does the display Screen Component that owns us have a linked Action Screen Component?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	bool HasLinkedActionScreenComponent() const;

	// Will ask the Action Screen Component linked to the owning Display Screen Component if this Widget's EntryData Can be executed upon. 
	// @return Will return results of Action Screen Component's CanExecuteAction() 
	// @return Will return false if the Owning ASC is invalid
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	bool CanExecuteAction();

	// Will execute an action using the Display Widget's EntryData on an Action Screen Component linked to the owning Display Screen Component
	// @Note: There are very few cases where a DisplayWidget will need to call this. The Action Screen Component has triggers for a DisplayWidget's Input, Hover, etc that should be used isntead. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	bool ExecuteAction();

	// Do we have text associated with the current last action result tag?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	bool HasTextAssociatedWithLastActionResultTag() const;

	// returns the text associated with the last action result tag that was set when either CanExecuteAction() or ExecuteAction() were called.
	// @return - Text from Map association.  Will be empty if no text is associated with the last action result tag. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	FText GetTextAssociatedWithLastActionResultTag() const;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget)
	FGameplayTag GetLastActionResult() const;

	// only called by the owning Display Screen Component
	void Release();

protected:
	// list out events that are user facing


	// Widget has just had it's entry data set
	// @index - The index this widget is in the entry set
	// @entry - the entry data
	// @todo - consider deprecating this function. 
	UFUNCTION(BlueprintImplementableEvent, Category = DisplayWidget)
	void OnEntryDataSet(int32 InIndex, const UObject* InEntry);

	// Handle any actions required before we receive a new set of data to represent.
	UFUNCTION(BlueprintImplementableEvent, Category = DisplayWidget)
	void OnReset();

	// event notifying that some type of input (Click, Button Press, etc) has happened on this widget
	UFUNCTION(BlueprintNativeEvent, Category = DisplayWidget)
	void OnInputAction();

	// these internal functions are broken out and made virtual in case an implementing class needs to override them. 
	virtual UObject* GetEntry_Internal() const { return Entry.Get(); }
	virtual void SetEntry_Internal(UObject* InEntry) { Entry = TStrongObjectPtr<UObject>(InEntry); }
	virtual void SetEntryData_Implementation(int32 InIndex, UObject* InEntry);
	virtual UObject* GetEntryData_Implementation() const;
	virtual int32 GetIndex_Implementation() const { return Index; }
	virtual void Reset_Implementation();
	bool CanExecuteAction_Implementation();
	bool HasLinkedActionScreenComponent_Implementation() const;
	UDisplayScreenComponent* GetOwningDisplayScreenComponent_Implementation() const;
	UActionScreenComponent* GetLinkedActionScreenComponent_Implementation() const;
	FGameplayTag GetLastActionResult_Implementation() const;
	bool HasTextAssociatedWithLastActionResultTag_Implementation() const;
	FText GetTextAssociatedWithLastActionResultTag_Implementation() const;

	// these take the existing widget event calls, and translates them to a version where we will know who broadcast them. 
	void SetFocus_Internal(bool bInFocused);
	void SetHovered_Internal(bool bInHovered);
	void SetSelected_Internal(bool bInSelected);
	void SetInputAction_Internal();


	TStrongObjectPtr<UObject> Entry;	// the entry data
	int32 Index = INDEX_NONE;			// what is the index of the widget in the view component array?
	TWeakObjectPtr<UDisplayScreenComponent> OwningDisplayScreenComponent; // View Component that is managing this widget

public:
	// list out deprecated functions. 
	// set the focus
	UE_DEPRECATED(Any, "Is Deprecated. Use UWidget:: Instead")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetFocus() is deprecated. Use UWidget::SetuserFocus()"))
	void SetFocus(bool bInFocused);
	// set if we are hovered
	UE_DEPRECATED(Any, "Is Deprecated. Try UUserWidget::OnMouseLeave()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetHovered() is deprecated. Try UUserWidget::OnMouseLeave()"))
	void SetHovered(bool bInHovered);
	// set if we are selected
	UE_DEPRECATED(Any, "Is Deprecated.  Use UCommonButtonBase::GetIsSelected()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetSelected() is deprecated. Use UCommonButtonBase::GetIsSelected()"))
	void SetSelected(bool bInSelected);

	// Force an input action to happen
	UE_DEPRECATED(Any, "Is Deprecated. UCommonButton::ExecuteTriggeredInput()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "ForceInputAction() is deprecated. Use UCommonButton::ExecuteTriggeredInput()"))
	void ForceInputAction();
	
	// are we focused?
	UE_DEPRECATED(Any, "Is Deprecated. Use UWidget::HasAnyUserFocus() instead")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta=(DeprecatedFunction, DeprecationMessage="IsFocused() is deprecated. Use UWidget::HasAnyUserFocus() instead"))
	bool IsFocused() const;


	// are we hovered?
	UE_DEPRECATED(Any, "Is Deprecated. Use UWidget::IsHovered()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "IsHovered() is deprecated. Use UWidget::IsHovered()"))
	bool IsHovered() const;

	// are we selected?
	UE_DEPRECATED(Any, "Is Deprecated. Use UCommonButtonBase::GetIsSelected()")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "IsSelected() is deprecated. Use UCommonButtonBase::GetIsSelected()"))
	bool IsSelected() const;

protected:
	// event notifying that the focus has changed
	UE_DEPRECATED(Any, "Is Deprecated. Bind to UCommonButtonBase::OnReceivedFocus or UCommonButtonBase::OnLostFocus ")
	UFUNCTION(BlueprintImplementableEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "OnFocusChanged() is deprecated. Bind to UCommonButtonBase::OnReceivedFocus or UCommonButtonBase::OnLostFocus "))
	void OnFocusChanged(bool bFocus);

	// event notifying that the selection of the widget has changed
	UE_DEPRECATED(Any, "Is Deprecated. Bind to UCommonButton::OnSelected/OnDeselected")
	UFUNCTION(BlueprintImplementableEvent, Category = DisplayWidget, Meta = (DeprecatedFunction, DeprecationMessage = "OnSelectionChanged() is deprecated. Bind to UCommonButton::OnSelected/OnDeselected"))
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

// these macros help define boilerplate code for UWidget classes that you want to implement IDisplayWidgetInterface
// This can be complicated because UUserWidget has different functions to define how it gets focus and hover differently than UButton
// 
// this defines what every implementation needs. 
#define DISPLAY_BASE_BOILERPLATE() \
public:\
UPROPERTY(BlueprintAssignable, Category = DisplayWidget)\
FViewAction OnInputAction;\
UPROPERTY(BlueprintAssignable, Category = DisplayWidget)\
FViewEvent OnSelectionChanged;\
UPROPERTY(BlueprintAssignable, Category = DisplayWidget)\
FViewEvent OnFocusChanged;\
UPROPERTY(BlueprintAssignable, Category = DisplayWidget)\
FViewEvent OnHoverChange;\
virtual FViewAction& GetOnAction() { return OnInputAction; }\
virtual FViewEvent& GetOnSelectionChange() { return OnSelectionChanged; }\
virtual FViewEvent& GetOnFocusChange() { return OnFocusChanged; }\
virtual FViewEvent& GetOnHoverChange() { return OnHoverChange; }\

//
// define boilerplate required for a UUserWidget
// @note: a UserWidget doesn't have Action or Selection built in, so we don't need to define those. 
#define DISPLAY_USERWIDGET_BOILERPLATE() \
DISPLAY_BASE_BOILERPLATE() \
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

// this macro defines boilerplate code for a class derived from CommonButtonBase that is required for implementing IDisplayWidgetInterface

#define DISPLAY_COMMONBUTTON_BOILERPLATE() \
DISPLAY_USERWIDGET_BOILERPLATE() \
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
#define DISPLAY_WIDGET_BOILERPLATE() DISPLAY_USERWIDGET_BOILERPLATE()
#define DISPLAY_BUTTON_AND_WIDGET_BOILERPLATE() DISPLAY_COMMONBUTTON_BOILERPLATE 