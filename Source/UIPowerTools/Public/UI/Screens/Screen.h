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
class UIPOWERTOOLS_API UScreen : public UCommonActivatableWidget, public IUICSAccessor, public IScreenInterface
{
	GENERATED_BODY()
public:
	UScreen(const FObjectInitializer& Initializer);

	friend class UCommonActivatableWidget;

	// UUserWidget begin
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	// UUserWidgetend

	// IScreenInterface begin
	virtual FScreenDelegate& GetOnScreenClose() { return OnScreenClose;};
	
	// called when the screen closes
	UPROPERTY(BlueprintAssignable)
	FScreenDelegate OnScreenClose;
	// IScreenInterface end

	// IUICSAccessor begin
	virtual UScreenComponent* GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const override;
	virtual TArray<UScreenComponent*> GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const override;
	virtual UScreenComponent* GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const override;
	// IUICSAccessor end

#if WITH_EDITOR
	UScreenComponent* GetScreenComponentFromGUID(const FGuid& Selector) const;
#endif

protected:


	UPROPERTY(EditAnywhere, Instanced, Export, Category = Screen, Meta = (DisplayPriority = 0, FullyExpand = true, ShowInnerProperties, ShowOnlyInnerProperties, DisplayName = "UI Component System"))
	TObjectPtr<UScreenComponentManager> ComponentManager = nullptr;
};
