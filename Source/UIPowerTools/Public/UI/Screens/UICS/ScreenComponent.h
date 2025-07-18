// Copyright 2025 Joel Gonzales

#pragma once

#include "Misc/Guid.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "ScreenComponent.generated.h"

// the base class for User Interface Component System (UICS) Components
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class UIPOWERTOOLS_API UScreenComponent : public UObject, public IUICSAccessor
{
	GENERATED_BODY()
public:
	virtual void Initialize() {}
	virtual void NativePreConstruct(bool bIsDesingTime) {}
	virtual void NativeConstruct() {}
	virtual void NativeDestruct() {}
	virtual void ReleaseSlateResources(bool bReleaseChildren) {}
	const FGuid& GetGuid() const;

#ifdef WITH_EDITORONLY_DATA
	virtual FString GetDisplayName() const;
	virtual FString GetDisplayNameVerbose() const;
#endif

protected:
	UPROPERTY(BlueprintReadWrite, Category = ScreenComponent)
	mutable FGuid Guid;
};
