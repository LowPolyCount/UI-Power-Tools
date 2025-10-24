// Copyright (c) Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UI/Screens/Screen.h"
#include "UICS/UICSTestActivatableWidget.h"
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
};

UCLASS(Hidden)
class UUICSTestActivatableWidgetHarness : public UUICSTestActivatableWidget
{
	GENERATED_BODY()
public:
	void AddComponent(UScreenComponent* Component) { GetComponentManager()->Add(Component); }

	// we don't have a viewport to add to in testing, so override to do nothing
	virtual void AddScreenToViewport(int32 ZOrder) {}
};

