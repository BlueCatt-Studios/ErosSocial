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
            "UMG",
            "Slate",
            "SlateCore",
            "Json",           // ← ADICIONAR!
            "JsonUtilities"   // ← ADICIONAR!
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });
    }
}