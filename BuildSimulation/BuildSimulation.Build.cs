// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BuildSimulation : ModuleRules
{
	public BuildSimulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "EnhancedInput", "UMG", "AIModule", "GameplayTasks" });
		PublicIncludePaths.AddRange(new string[] {"BuildSimulation"});
	}
}
