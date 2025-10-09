// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "IUICSAccessor.generated.h"

class UScreen;
class UScreenComponent;
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
	virtual UScreen* GetScreen() const;
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