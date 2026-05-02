// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "ScreenComponentManager.generated.h"

class UScreenComponent;
struct FComponentSelector;
struct FGuid;

// manages UICS Components on a screen
UCLASS(CollapseCategories, Meta=(DisplayName = "UI Component System"))
class UIPOWERTOOLS_API UScreenComponentManager : public UScreenComponentWorldContext
{
	GENERATED_BODY()
public:

	void Add(UScreenComponent* Component);
	UScreenComponent* GetComponent(TSubclassOf<UScreenComponent> Type) const;
	TArray<UScreenComponent*> GetAllComponents(TSubclassOf<UScreenComponent> Type) const;
	UScreenComponent* GetComponentByName(const FName Name, TSubclassOf<UScreenComponent> Type) const;
	UScreenComponent* GetComponentFromSelector(const FComponentSelector& Selector) const;
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const;
	UScreenComponent* GetComponentFromGUID(const FGuid& Selector) const;
#endif

	void Initialize();
	void NativePreConstruct(bool bIsDesignTime);
	void NativeConstruct();
	void NativeDestruct();

protected:
	UPROPERTY(EditAnywhere, Instanced, Export, Category = ScreenComponent, Meta = (ShowInnerProperties))
	TArray<TObjectPtr<UScreenComponent>> Components;
};
