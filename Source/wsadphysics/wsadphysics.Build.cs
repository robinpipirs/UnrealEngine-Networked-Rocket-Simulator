// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class wsadphysics : ModuleRules
{
	public wsadphysics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "UMG", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		PublicDependencyModuleNames.AddRange(new string[] { "EnhancedInput", "SlateCore", "Slate" });
		PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem",
			"OnlineSubsystemUtils", "OnlineSubsystemNull", "OnlineSubsystemSteam" });
	}
}
