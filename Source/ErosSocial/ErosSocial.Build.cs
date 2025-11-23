// Copyright BlueCatt, Inc. All Rights Reserved.

// ErosSocial.Build.cs

using UnrealBuildTool;

public class ErosSocial : ModuleRules
{
    public ErosSocial(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",           // <- ADICIONAR ESTA LINHA
            "Slate",         // <- ADICIONAR ESTA LINHA
            "SlateCore"      // <- ADICIONAR ESTA LINHA
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}