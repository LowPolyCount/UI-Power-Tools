// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
#include "UICSTestActivatableWidget.generated.h"

/**
 * An example on how to add UICS to an activatable widget. Used in our unit tests
 */
UCLASS(Hidden)
class UUICSTestActivatableWidget : public UCommonActivatableWidget, public IUICSScreenAccessor
{
	GENERATED_BODY()

public:
	// in the constructor, we create a default ComponentManager so that it will look correct in the editor
	UUICSTestActivatableWidget(const FObjectInitializer& Initializer) :Super(Initializer)
	{
		ComponentManager = Initializer.CreateDefaultSubobject<UScreenComponentManager>(this, TEXT("ScreenComponentManager"));
	}


	// everything below is what is needed to integrate screen components
	// UUserWidget begin
	virtual bool Initialize() override {IUICSScreenAccessor::Initialize(); return Super::Initialize();}
	virtual void NativePreConstruct() override { Super::NativePreConstruct(); IUICSScreenAccessor::WidgetNativePreConstruct(IsDesignTime()); }
	virtual void NativeConstruct() override { Super::NativeConstruct(); IUICSScreenAccessor::NativeConstruct();}
	virtual void NativeDestruct() override { Super::NativeDestruct(); IUICSScreenAccessor::NativeDestruct();}
	// UUserWidget end

	// UCommonActivatableWidget begin
	virtual UWidget* NativeGetDesiredFocusTarget() const override
	{
		// if we haven't overridden this in BP, then get the focus from the view component
		return (GetClass()->IsFunctionImplementedInScript(FName(L"BP_GetDesiredFocusTarget")))
		? Super::NativeGetDesiredFocusTarget()
		: IUICSScreenAccessor::GetDesiredFocusTargetFromViewComponents();
	}
	// UCommonActivatableWidget end
	
protected:
	// IUICSScreenAccessor begin
	virtual UScreenComponentManager* GetComponentManager() const { return ComponentManager; }

	// the manager of our screen components
	UPROPERTY(EditAnywhere, Instanced, Export, Category = Screen, Meta = (DisplayPriority = 0, FullyExpand = true, ShowInnerProperties, ShowOnlyInnerProperties, DisplayName = "UI Component System"))
	TObjectPtr<UScreenComponentManager> ComponentManager = nullptr;
	// IUICSScreenAccessor end
};

