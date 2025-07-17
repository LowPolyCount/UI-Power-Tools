// Copyright (c) Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "UI/Screens/Screen.h"
#include "UI/Screens/UICS/ScreenComponentManager.h"
#include "TestHelpers.generated.h"

class UScreenComponent;

// this file contains helpers for us to test our classes

UCLASS(Hidden)
class UScreenHarness : public UScreen
{
	GENERATED_BODY()
public:
	void AddComponent(UScreenComponent* Component) { ComponentManager->Add(Component); }

	// we don't have a viewport to add to in testing, so override to do nothing
	virtual void AddScreenToViewport(int32 ZOrder) override {}

protected:
	//TSharedRef<SWidget> TestSWidget;
};