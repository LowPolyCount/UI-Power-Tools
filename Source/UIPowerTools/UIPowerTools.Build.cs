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
				"CoreUObject",
				"CommonInput",
				"CommonUI",
				"DeveloperSettings",
				"Engine",
				"EnhancedInput",
				"GameplayTags",
				"Slate",
				"SlateCore",
				"UMG",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{


			});		
	}
}
