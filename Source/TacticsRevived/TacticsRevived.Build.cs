// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TacticsRevived : ModuleRules
{
	public TacticsRevived(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "DedicatedServers",
            "OnlineSubsystemSteam",
            "OnlineSubsystem",
            "GameplayAbilities"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AIModule",
            "Slate",
            "SlateCore",
            "GameplayTags",
            "GameplayTasks",
        });
    }
}
