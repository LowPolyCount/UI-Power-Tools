// Copyright (c) Joel Gonzales

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class UIPowerToolsTest : ModuleRules
{
	public UIPowerToolsTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"UnrealEd",
				"CommonUI",
				"EnhancedInput",
				"GameplayTags",
			});
			
		
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"UIPowerTools",
			});
	}
}
