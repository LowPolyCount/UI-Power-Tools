// Copyright 2025 Joel Gonzales

#pragma once

#include "Misc/Guid.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "UI/Screens/UICS/ScreenComponentWorldContext.h"
#include "ScreenComponent.generated.h"

// the base class for User Interface Component System (UICS) Components
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class UIPOWERTOOLS_API UScreenComponent : public UScreenComponentWorldContext, public IUICSAccessor
{
	GENERATED_BODY()
public:
	UScreenComponent(const FObjectInitializer& Initializer);
	virtual void Initialize() {}
	virtual void NativePreConstruct(bool bIsDesingTime) {}
	virtual void NativeConstruct() {}
	virtual void NativeDestruct() {}
	virtual void ReleaseSlateResources(bool bReleaseChildren) {}
	const FGuid& GetGuid() const;
	const FName& GetComponentName() const;

#ifdef WITH_EDITORONLY_DATA
	virtual FString GetDisplayName() const;
	virtual FString GetDisplayNameVerbose() const;
#endif

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ScreenComponent)
	//FName ComponentName;
	UPROPERTY(BlueprintReadOnly, Category = ScreenComponent)
	mutable FGuid Guid;
};
