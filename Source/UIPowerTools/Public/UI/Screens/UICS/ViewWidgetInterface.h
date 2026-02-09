// Copyright 2025 Joel Gonzales

#pragma once

#include "UObject/Interface.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ViewWidgetInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewAction, TScriptInterface<IViewWidgetInterface>, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewEvent, TScriptInterface<IViewWidgetInterface>, Widget, bool, bGained);


// the purpose of this interface is to get around the issue with UWidget events where we don't know who 
// Example: If you're an outside class listening to multiple Button's OnClick events, you don't know which button broadcast OnClick. 
// ListView gets around this because it can create it's own SWidget where Slate will tell it which Widget was Clicked/Selected/Focused/etc. 
// We don't have that luxury because View Screen Component needs to be computable with all UPanels.
// Widgets are required to implement this to be usable with a view component.
UINTERFACE(BlueprintType)
class UViewWidgetInterface : public UInterface
{
	GENERATED_BODY()
};


class UIPOWERTOOLS_API IViewWidgetInterface
{
	GENERATED_BODY()

protected:
	// first list the functions that are user facing. Because this is an interface, we need to list all BlueprintCallable functions as BlueprintNativeEvent even if we don't want to. 
	
	
	// this is where the data for this widget has been set. 
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void Populate(UObject* EntryData);

	// get the current entry data
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	UObject* GetEntryData() const;

		// get the widget's index on the panel
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	int32 GetIndex() const;

protected:
	// list out events that are user facing

	// Widget has just had it's entry data set
	// @index - The index this widget is in the entry set
	// @entry - the entry data
	// @todo - consider deprecating this function. 
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnEntryDataSet(int32 InIndex, const UObject* InEntry);

	// Widget is being reset
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnReset();

	// event notifying that input has been applied to this widget
	UFUNCTION(BlueprintNativeEvent, Category = ViewWidget)
	void OnInputAction();


public:
	// these delegates need to be created in the implementing class so that they can be set as blueprint assignable
	virtual FViewAction& GetOnAction() = 0;
	virtual FViewEvent& GetOnSelectionChange() = 0;
	virtual FViewEvent& GetOnFocusChange() = 0;
	virtual FViewEvent& GetOnHoverChange() = 0;

	// set the widget's entry data.  This is only callable in blueprint for convenience. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void SetEntryData(int32 InIndex, UObject* InEntry);

	// reset the widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void Reset();

protected:

	// these internal functions are broken out in case an implementing class needs to override them. 
	virtual UObject* GetEntry_Internal() const { return Entry.Get(); }
	virtual void SetEntry_Internal(UObject* InEntry) { Entry = TStrongObjectPtr<UObject>(InEntry); }


	void SetEntryData_Implementation(int32 InIndex, UObject* InEntry);
	UObject* GetEntryData_Implementation() const;
	int32 GetIndex_Implementation() const { return Index; }
	void Reset_Implementation();


	void SetFocus_Internal(bool bInFocused);
	void SetHovered_Internal(bool bInHovered);
	void SetSelected_Internal(bool bInSelected);
	void SetInputAction_Internal();


	TStrongObjectPtr<UObject> Entry;	// the entry data
	int32 Index = INDEX_NONE;			// what is the index of the widget in the view component array?

public:
	// list out deprecated functions. 
	// set the focus
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetFocus() is deprecated. Use UWidget::SetuserFocus()"))
	void SetFocus(bool bInFocused);
	// set if we are hovered
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetHovered() is deprecated. Try UUserWidget::OnMouseLeave()"))
	void SetHovered(bool bInHovered);
	// set if we are selected
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "SetSelected() is deprecated. Use UCommonButtonBase::GetIsSelected()"))
	void SetSelected(bool bInSelected);

	// Force an input action to happen
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "ForceInputAction() is deprecated. Use UCommonButton::ExecuteTriggeredInput()"))
	void ForceInputAction();
	
	// are we focused?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta=(DeprecatedFunction, DeprecationMessage="IsFocused() is deprecated. Use UWidget::HasAnyUserFocus() instead"))
	bool IsFocused() const;


	// are we hovered?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "IsHovered() is deprecated. Use UWidget::IsHovered()"))
	bool IsHovered() const;

	// are we selected?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "IsSelected() is deprecated. Use UCommonButtonBase::GetIsSelected()"))
	bool IsSelected() const;

protected:
	// event notifying that the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget, Meta = (DeprecatedFunction, DeprecationMessage = "OnFocusChanged() is deprecated. Bind to UCommonButtonBase::OnReceivedFocus or UCommonButtonBase::OnLostFocus "))
	void OnFocusChanged(bool bFocus);

	// event notifying that the selection of the widget has changed
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

// keep old definitions for backwards compatability
#define VIEW_WIDGET_BOILERPLATE() VIEW_USERWIDGET_BOILERPLATE()
#define VIEW_BUTTON_AND_WIDGET_BOILERPLATE() VIEW_COMMONBUTTON_BOILERPLATE 