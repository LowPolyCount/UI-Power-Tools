// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * This is the module definition for the editor mode. You can implement custom functionality
 * as your plugin module starts up and shuts down. See IModuleInterface for more extensibility options.
 */
class FUICSTestModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
