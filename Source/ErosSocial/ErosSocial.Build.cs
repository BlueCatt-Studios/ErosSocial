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

        PublicIncludePaths.AddRange(new string[]
        {
            System.IO.Path.Combine(ModuleDirectory, "Data"),
            System.IO.Path.Combine(ModuleDirectory, "Systems/SaveSystem")
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });
    }
}