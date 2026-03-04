// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIPTStatics.generated.h"

class IUICSScreenAccessor;
class UWidget;

/**
 * A place for Static Functions used in UI Power Tools
 */
UCLASS()
class UIPOWERTOOLS_API UUIPTStatics : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
public:
	// get the first screen component of the given type
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	// @Type - The Type of Component you are getting
	UFUNCTION(BlueprintPure, Category = ScreenComponent, Meta=(DefaultToSelf="PathToScreen", DeterminesOutputType = "Type", DisplayName = "Get Screen Component"))
	static UScreenComponent* GetScreenComponent_BP(const UObject* PathToScreen, TSubclassOf<UScreenComponent> Type);

	// get all screen components of a type
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	// @Type - The Type of Component you are getting
	UFUNCTION(BlueprintPure, Category = ScreenComponent, Meta = (DefaultToSelf = "PathToScreen", DeterminesOutputType = "Type", DisplayName = "Get All Screen Components"))
	static TArray<UScreenComponent*> GetAllScreenComponents_BP(const UObject* PathToScreen, TSubclassOf<UScreenComponent> Type);

	// get a screen component by Name
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	// @Name - the FName of the component
	// @Type - The Type of Component you are getting
	UFUNCTION(BlueprintPure, Category = ScreenComponent, Meta = (DefaultToSelf = "PathToScreen", DeterminesOutputType = "Type", DisplayName = "Get Screen Component By Name"))
	static UScreenComponent* GetScreenComponentByName_BP(const UObject* PathToScreen, FName Name, TSubclassOf<UScreenComponent> Type);

	// get a screen component through a component selector
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	// @Selector - The Component Selector that is set in the editor to a specific component
	// @Type - The Type of Component you are getting
	UFUNCTION(BlueprintPure, Category = ScreenComponent, Meta = (DefaultToSelf = "PathToScreen", DeterminesOutputType = "Type", DisplayName = "Get Screen Component From Selector"))
	static UScreenComponent* GetScreenComponentFromSelector_BP(const UObject* PathToScreen, const FComponentSelector& Selector, TSubclassOf<UScreenComponent> Type);

	// C++ Versions of the GetScreenComponent() functions using Templates for the Types

	// get the first screen component of the given type
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	template<class ComponentType>
	static ComponentType* GetScreenComponent(const UObject* PathToScreen);

	// get all screen components of a type
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	template<class ComponentType>
	static TArray<ComponentType*> GetAllScreenComponents(const UObject* PathToScreen);

	// get a screen component by Name
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	// @Name - the FName of the component
	template<class ComponentType>
	static ComponentType* GetScreenComponentByName(const UObject* PathToScreen, FName Name);

	// get a screen component through a component selector
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	// @Selector - The Component Selector that is set in the editor to a specific component
	template<class ComponentType>
	static ComponentType* GetScreenComponentFromSelector(const UObject* PathToScreen, const FComponentSelector& Selector);

	// Search all the View Components that are set for initial focus to get the first widget that is focusable
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	static UWidget* GetDesiredFocusTargetFromViewComponents(const UObject* PathToScreen);

	// get the screen (IE, the implementor of IUICSScreenAccessor) that holds all of our Screen Components
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components
	static TScriptInterface<const IUICSScreenAccessor> GetScreenAccessor(const UObject* PathToScreen);

	// non-const version of GetScreenAccessor
	// get the screen (IE, the implementor of IUICSScreenAccessor) that holds all of our Screen Components
	// @PathToScreen - A Widget or Screen Component that has an Outer/Parent chain to or is a Screen that holds the Screen Components

	static TScriptInterface<IUICSScreenAccessor> GetScreenAccessor(UObject* PathToScreen);
};


template<class ComponentType>
ComponentType* UUIPTStatics::GetScreenComponent(const UObject* PathToScreen)
{
	return Cast<ComponentType>(GetScreenComponent_BP(PathToScreen, TSubclassOf<UScreenComponent>(ComponentType::StaticClass())));
}

template<class ComponentType>
TArray<ComponentType*> UUIPTStatics::GetAllScreenComponents(const UObject* PathToScreen)
{
	TArray<ComponentType*> RetVal;
	for(UScreenComponent* Component : GetAllScreenComponents_BP(PathToScreen, TSubclassOf<UScreenComponent>(ComponentType::StaticClass())))
	{
		if (ComponentType* AsType = Cast<ComponentType>(Component))
		{
			RetVal.Emplace(AsType);
		}
	}
	return RetVal;
}

template<class ComponentType>
ComponentType* UUIPTStatics::GetScreenComponentByName(const UObject* PathToScreen, FName Name)
{
	return Cast<ComponentType>(GetScreenComponentByName_BP(PathToScreen, Name, TSubclassOf<UScreenComponent>(ComponentType::StaticClass())));
}

template<class ComponentType>
ComponentType* UUIPTStatics::GetScreenComponentFromSelector(const UObject* PathToScreen, const FComponentSelector& Selector)
{
	return Cast<ComponentType>(GetScreenComponentFromSelector_BP(PathToScreen, Selector, TSubclassOf<ComponentType>()));
}	
