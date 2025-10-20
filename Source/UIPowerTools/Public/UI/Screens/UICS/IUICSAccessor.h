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

// allows an object with an outer path to the owning screen to access UICS Screen Components
class UIPOWERTOOLS_API IUICSAccessor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta=(DisplayName = "GetScreenComponent", DeterminesOutputType="Type"))
	virtual UScreenComponent* GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const;
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta = (DisplayName = "GetAllScreenComponents", DeterminesOutputType = "Type"))
	virtual TArray<UScreenComponent*> GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const;
	UFUNCTION(BlueprintCallable, Category = ScreenComponent, Meta = (DisplayName = "GetScreenComponentFromSelector", DeterminesOutputType = "Type"))
	virtual UScreenComponent* GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const;

	template<class ComponentType>
	ComponentType* GetScreenComponent() const;
	template<class ComponentType>
	TArray<ComponentType*> GetAllScreenComponents() const;
	template<class ComponentType>
	ComponentType* GetScreenComponentFromSelector(const FComponentSelector& Selector) const;

protected:
	virtual TScriptInterface<IUICSScreenAccessor> GetScreenAccessor() const;
};

UINTERFACE(MinimalAPI, NotBlueprintable)
class UUICSScreenAccessor : public UUICSAccessor
{
	GENERATED_BODY()
};

// Signifies the terminal node when looking up a component. 
class UIPOWERTOOLS_API IUICSScreenAccessor : public IUICSAccessor
{
	GENERATED_BODY()
public:
	virtual UScreenComponent* GetScreenComponent_BP(TSubclassOf<UScreenComponent> Type) const override;
	virtual TArray<UScreenComponent*> GetAllScreenComponents_BP(TSubclassOf<UScreenComponent> Type) const override;
	virtual UScreenComponent* GetScreenComponentFromSelector_BP(const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type) const override;

	// these functions correspond to and should be called with the implementor's UUserWidget Functions
	// Initialize will need to return bool since UserWidget is the most likely implementor of this
	// the bool does not mean anything
	virtual bool Initialize();
	virtual void NativePreConstruct(bool bIsDesignTime);
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	virtual UWidget* GetDesiredFocusTargetFromViewComponents() const;

#if WITH_EDITOR
	UScreenComponent* GetScreenComponentFromGUID(const FGuid& Selector) const;
#endif

protected:
	virtual UScreenComponentManager* GetComponentManager() const = 0;
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
ComponentType* IUICSAccessor::GetScreenComponentFromSelector(const FComponentSelector& Selector) const
{
	return Cast<ComponentType>(GetScreenComponentFromSelector_BP(Selector, TSubclassOf<ComponentType>()));
}