// Copyright (c) Joel Gonzales

using UnrealBuildTool;

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
                "EnhancedInput"
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
