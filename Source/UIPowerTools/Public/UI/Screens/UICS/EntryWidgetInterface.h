// Copyright 2025 Joel Gonzales

#pragma once

#include "UObject/Interface.h"
#include "EntryWidgetInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewAction, TScriptInterface<IEntryWidgetInterface>, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewEvent, TScriptInterface<IEntryWidgetInterface>, Widget, bool, bGained);


UINTERFACE(BlueprintType)
class UEntryWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

// allows a widget to be used with a view component
class UIPOWERTOOLS_API IEntryWidgetInterface
{
	GENERATED_BODY()
public:
	// these delegates need to be created in the implementing class so that they can be set as blueprint assignable
	virtual FViewAction& GetOnAction() = 0;
	virtual FViewEvent& GetOnSelectionChange() = 0;
	virtual FViewEvent& GetOnFocusChange() = 0;
	virtual FViewEvent& GetOnHoverChange() = 0;

	// set the widget's entry data
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void SetEntryData(int32 InIndex, UObject* InEntry);

	// get the current entry data
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	UObject* GetEntryData() const;

	// reset the widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void Reset();

	// get the widget's index in the set
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	int32 GetIndex() const;

	// are we focused?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	bool IsFocused() const;

	// set the focus
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void SetFocus(bool bInFocused);

	// are we hovered?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	bool IsHovered() const;

	// set if we are hovered
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void SetHovered(bool bInHovered);

	// are we selected?
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	bool IsSelected() const;

	// set if we are selected
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void SetSelected(bool bInSelected);

	// Force an input action to happen
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ViewWidget)
	void ForceInputAction();

protected:
	// Widget has just had it's entry data set
	// @index - The index this widget is in the entry set
	// @entry - the entry data
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnEntryDataSet(int32 InIndex, const UObject* InEntry);

	// Widget is being reset
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnReset();

	// event notifying that the focus has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnFocusChanged(bool bFocus);

	// event notifying that the selection of the widget has changed
	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void OnSelectionChanged(bool bInSelected);

	// event notifying that input has been applied to this widget
	UFUNCTION(BlueprintNativeEvent, Category = ViewWidget)
	void OnInputAction();

	UFUNCTION(BlueprintImplementableEvent, Category = ViewWidget)
	void Populate(UObject* EntryData);

	// these internal functions are broken out in case an implementing class needs to override them. 
	virtual UObject* GetEntry_Internal() const { return Entry.Get(); }
	virtual void SetEntry_Internal(UObject* InEntry) { Entry = TStrongObjectPtr<UObject>(InEntry); }


	void SetEntryData_Implementation(int32 InIndex, UObject* InEntry);
	UObject* GetEntryData_Implementation() const;
	int32 GetIndex_Implementation() const { return Index; }
	bool IsFocused_Implementation() const;
	void SetFocus_Implementation(bool bInFocused);
	bool IsHovered_Implementation() const;
	void SetHovered_Implementation(bool bInHovered);
	void SetSelected_Implementation(bool bInSelected);
	bool IsSelected_Implementation() const;
	void ForceInputAction_Implementation();
	void OnInputAction_Implementation();
	void Reset_Implementation();

	TStrongObjectPtr<UObject> Entry;	// the entry data
	int32 Index = INDEX_NONE;			// what is the index of the widget in the view component array?
	bool bSelected = false;				// is the widget in a selected state?
	bool bFocused = false;				// is the widget focused?
	bool bHovered = false;				// is the widget hovered?
};

// this macro can help define boilerplate code required for implementing IEntryWidgetInterface
#define VIEW_WIDGET_BOILERPLATE() \
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
virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override\
{\
	const FReply RetVal = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);\
	IEntryWidgetInterface::Execute_SetFocus(this, true);\
	return RetVal;\
}\
virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override\
{\
	Super::NativeOnFocusLost(InFocusEvent);\
	IEntryWidgetInterface::Execute_SetFocus(this, false);\
}