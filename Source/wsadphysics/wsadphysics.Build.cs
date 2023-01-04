// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class wsadphysics : ModuleRules
{
	public wsadphysics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
