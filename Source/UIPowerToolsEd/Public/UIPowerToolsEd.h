// Copyright (c) Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUIPowerToolsEdModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
