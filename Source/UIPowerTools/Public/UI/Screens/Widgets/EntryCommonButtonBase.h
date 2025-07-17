// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "UI/Screens/UICS/EntryWidgetInterface.h"
#include "UI/Screens/UICS/IUICSAccessor.h"
#include "EntryCommonButtonBase.generated.h"

// a Common Button Base that can be used as an entry widget
UCLASS()
class UIPOWERTOOLS_API UEntryCommonButtonBase : public UCommonButtonBase, public IEntryWidgetInterface, public IUICSAccessor
{
	GENERATED_BODY()

public:
	VIEW_WIDGET_BOILERPLATE();

	virtual void NativeOnSelected(bool bBroadcast) override
	{
		Super::NativeOnSelected(bBroadcast);
		Execute_SetSelected(this, true);
	}

	virtual void NativeOnDeselected(bool bBroadcast) override
	{
		Super::NativeOnDeselected(bBroadcast);
		Execute_SetSelected(this, false);
	}
	
	virtual void HandleFocusReceived() override
	{
		Super::HandleFocusReceived();
		Execute_SetFocus(this, true);
	}
	
	virtual void HandleFocusLost() override
	{
		Super::HandleFocusLost();
		Execute_SetFocus(this, false);
	}

	virtual void NativeOnHovered() override
	{
		Super::NativeOnHovered();
		Execute_SetHovered(this, true);
	}

	virtual void NativeOnUnhovered() override
	{
		Super::NativeOnUnhovered();
		Execute_SetHovered(this, false);
	}

	virtual void NativeOnActionComplete() override
	{
		Super::NativeOnActionComplete();
		Execute_OnInputAction(this);
	}

	virtual void NativeOnClicked() override
	{
		Super::NativeOnClicked();
		Execute_OnInputAction(this);
	}
};
