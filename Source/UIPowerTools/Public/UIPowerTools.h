// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


 DECLARE_LOG_CATEGORY_EXTERN(LogUICS, Log, All)

class FUIPowerToolsModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
