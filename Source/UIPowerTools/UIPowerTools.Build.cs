// Copyright 2025 Joel Gonzales

using UnrealBuildTool;

public class UIPowerTools : ModuleRules
{
	public UIPowerTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"CommonUI",
				"GameplayTags",
				"UMG",
			});		
	}
}
