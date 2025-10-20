// Copyright 2025 Joel Gonzales

#pragma once

#include "CommonActivatableWidget.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/ScreenInterface.h"
#include "UI/Screens/UICS/InputScreenComponent.h"
#include "Screen.generated.h"

class UScreenComponentManager;
class UInputAction;
struct FGuid;
struct FUIInputConfig;

// a screen is the base unit of our UI System, it can have widgets, controls, etc attached to it as part of a hierarchy to display visual elements
// it has an array of UICS Components that can be used to add functionality
UCLASS(BlueprintType, Blueprintable, Abstract, meta = (PrioritizeCategories = "Screen"))
class UIPOWERTOOLS_API UScreen : public UCommonActivatableWidget, public IUICSScreenAccessor, public IScreenInterface
{
	GENERATED_BODY()
public:
	UScreen(const FObjectInitializer& Initializer);
	
	// called when the screen closes
	UPROPERTY(BlueprintAssignable)
	FScreenDelegate OnScreenClose;

	// IScreenInterface begin
	virtual FScreenDelegate& GetOnScreenClose() { return OnScreenClose;};
	// IScreenInterface end

	// UUserWidget begin
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	// UUserWidget end

protected:
	// IUICSAccessor begin
	virtual UScreenComponentManager* GetComponentManager() const { return ComponentManager; }
	// IUICSAccessor end

	// the manager of our screen components
	UPROPERTY(EditAnywhere, Instanced, Export, Category = Screen, Meta = (DisplayPriority = 0, FullyExpand = true, ShowInnerProperties, ShowOnlyInnerProperties, DisplayName = "UI Component System"))
	TObjectPtr<UScreenComponentManager> ComponentManager = nullptr;
};
