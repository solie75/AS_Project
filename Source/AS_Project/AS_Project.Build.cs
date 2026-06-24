// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AS_Project : ModuleRules
{
	public AS_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"PoseSearch",
			"AnimationLocomotionLibraryRuntime",
			"MotionWarping",
			"Chooser",
			"GameplayCameras",
			"AnimGraphRuntime"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "AnimationWarpingRuntime", "GameplayTags" });

		PublicIncludePaths.AddRange(new string[] {
			"AS_Project",
			"AS_Project/Variant_Platforming",
			"AS_Project/Variant_Platforming/Animation",
			"AS_Project/Variant_Combat",
			"AS_Project/Variant_Combat/AI",
			"AS_Project/Variant_Combat/Animation",
			"AS_Project/Variant_Combat/Gameplay",
			"AS_Project/Variant_Combat/Interfaces",
			"AS_Project/Variant_Combat/UI",
			"AS_Project/Variant_SideScrolling",
			"AS_Project/Variant_SideScrolling/AI",
			"AS_Project/Variant_SideScrolling/Gameplay",
			"AS_Project/Variant_SideScrolling/Interfaces",
			"AS_Project/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
