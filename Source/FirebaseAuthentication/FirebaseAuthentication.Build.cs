/**
 *	Copyright (C) 2021. Nikita Klimov. All rights reserved.
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the Apache License Version 2.0.
 * 
 *	See file LICENSE for further informations on licensing terms.
 */

using UnrealBuildTool;

public class FirebaseAuthentication : ModuleRules
{
	public FirebaseAuthentication(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

        string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("Launch");
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "FirebaseAuthentication_UPL_Android.xml"));
        }
    }
}
