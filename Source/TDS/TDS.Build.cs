// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TDS : ModuleRules
{
	public TDS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","PhysicsCore", "NavigationSystem", "AIModule" });
       // PublicIncludePaths.AddRange(new string[] { "TDS/Source/TDS/Game" });
    }
}
