// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "IUICSAccessor.generated.h"

class UScreen;
class UScreenComponent;
class IUICSScreenAccessor;
class UScreenComponentManager;
struct FUICSSelector;

UINTERFACE(MinimalAPI, NotBlueprintable)
class UUICSAccessor : public UInterface
{
	GENERATED_BODY()
};

// IUICSAccessor has been deprecated and it's functions moved to UIPTStatics so that all widgets with an outer path to a screen can use them, not just implementors of the class. 
// allows an object with an outer path to the owning screen to access UICS Screen Components
class UIPOWERTOOLS_API IUICSAccessor
{
	GENERATED_BODY()
public:
	// get the first screen component of the given type
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetScreenComponent_BP Instead")
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta=(DisplayName = "Get Screen Component", DeterminesOutputType = "Type", DeprecatedFunction, DeprecationMessage = "Switch to UIPTStatics::GetScreenComponent()"))
	virtual UScreenComponent* GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const;

	// get all screen components of type
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetAllScreenComponents_BP Instead")
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta = (DisplayName = "Get All Screen Components", DeterminesOutputType = "Type", DeprecatedFunction, DeprecationMessage = "Switch to UIPTStatics::GetAllScreenComponents()"))
	virtual TArray<UScreenComponent*> GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const;

	// get a screen component by Name
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetScreenComponentByName_BP Instead")
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta = (DisplayName = "Get Screen Component By Name", DeterminesOutputType = "Type", DeprecatedFunction, DeprecationMessage = "Switch to UIPTStatics::GetScreenComponentByName()"))
	virtual UScreenComponent* GetScreenComponentByName_BP(const FName Name, TSubclassOf<UScreenComponent> Type) const;

	// get a screen component through a component selector 
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetScreenComponentFromSelector_BP Instead")
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta = (DisplayName = "Get ScreenComponent From Selector", DeterminesOutputType = "Type", DeprecatedFunction, DeprecationMessage = "Switch to UIPTStatics::GetScreenComponentFromSelector()"))
	virtual UScreenComponent* GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const;


	
	template<class ComponentType>
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetScreenComponent Instead")
	ComponentType* GetScreenComponent() const;

	
	template<class ComponentType>
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetAllScreenComponents Instead")
	TArray<ComponentType*> GetAllScreenComponents() const;

	
	template<class ComponentType>
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetScreenComponentByName Instead")
	ComponentType* GetScreenComponentByName(const FName& Name) const;

	
	template<class ComponentType>
	UE_DEPRECATED(Any, "Is Deprecated. Use UUIPTStatics::GetScreenComponentFromSelector Instead")
	ComponentType* GetScreenComponentFromSelector(const FComponentSelector& Selector) const;

protected:
	virtual TScriptInterface<IUICSScreenAccessor> GetScreenAccessor() const;
};

UINTERFACE(MinimalAPI, NotBlueprintable)
class UUICSScreenAccessor : public UUICSAccessor
{
	GENERATED_BODY()
};

// Implemented by a widget that represents a Screen - the root of your UI that will be displayed. 
// The screen contains the master list of Screen Components being used.
class UIPOWERTOOLS_API IUICSScreenAccessor : public IUICSAccessor
{
	GENERATED_BODY()
public:
	// you will need to add a UScreenComponentManager* property in your class. We Recommend the following UProperty Tags: 
	/*
		UPROPERTY(EditAnywhere, Instanced, Export, Category = Screen, Meta = (DisplayPriority = 0, FullyExpand = true, DisplayName = "UI Component System"))
		TObjectPtr<UScreenComponentManager> ComponentManager = nullptr;
	*/
	virtual UScreenComponentManager* GetComponentManager() const = 0;

	// these functions correspond to and should be called in the implementor's UUserWidget Functions of the same name. 
	// Example:
	/*bool UImplementingWidget::Initialize()
	{
		const bool RetVal = Super::Initialize();
		IUICSScreenAccessor::Initialize();
		return RetVal;
	}*/
	virtual bool Initialize();	// Initialize returns bool just to match UserWidget's definition of the function. the bool does not mean anything in this context
	virtual void WidgetNativePreConstruct(bool bIsDesignTime);
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	// end function correspond


	virtual UWidget* GetDesiredFocusTargetFromViewComponents() const;
#if WITH_EDITOR
	UScreenComponent* GetScreenComponentFromGUID(const FGuid& Selector) const;
#endif

	// begin non-user facing functions
public:
	// while the base versions of these functions are deprecated, the overrides are not as they specify how a Screen retrieves a component from it's list.
	// However, they should only be called from the UIPTStatics Get Component functions
	// @todo: When IUICSAccessor is removed, consider renaming functions, friending to UIPTStatics
	virtual UScreenComponent* GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const override;
	virtual TArray<UScreenComponent*> GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const override;
	virtual UScreenComponent* GetScreenComponentByName_BP(const FName Name, TSubclassOf<UScreenComponent> Type) const override;
	virtual UScreenComponent* GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const override;

};


template<class ComponentType>
ComponentType* IUICSAccessor::GetScreenComponent() const
{
	return Cast<ComponentType>(GetScreenComponent_BP(TSubclassOf<UScreenComponent>(ComponentType::StaticClass())));
}

template<class ComponentType>
TArray<ComponentType*> IUICSAccessor::GetAllScreenComponents() const
{
	TArray<ComponentType*> RetVal;
	for(UScreenComponent* Component : GetAllScreenComponents_BP(TSubclassOf<UScreenComponent>(ComponentType::StaticClass())))
	{
		if (ComponentType* AsType = Cast<ComponentType>(Component))
		{
			RetVal.Emplace(AsType);
		}
	}
	return RetVal;
}

template<class ComponentType>
ComponentType* IUICSAccessor::GetScreenComponentByName(const FName& Name) const
{
	return Cast<ComponentType>(GetScreenComponentByName_BP(Name, TSubclassOf<UScreenComponent>(ComponentType::StaticClass())));
}

template<class ComponentType>
ComponentType* IUICSAccessor::GetScreenComponentFromSelector(const FComponentSelector& Selector) const
{
	return Cast<ComponentType>(GetScreenComponentFromSelector_BP(Selector, TSubclassOf<ComponentType>()));
}