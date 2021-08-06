/**
 *	Copyright (C) 2021. Nikita Klimov. All rights reserved.
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the Apache License Version 2.0.
 * 
 *	See file LICENSE for further informations on licensing terms.
 */

#include "FirebaseAuthentication.h"
#include "FirebaseSettings.h"
#include "Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FFirebaseAuthenticationModule"

void FFirebaseAuthenticationModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			TEXT("Project"), 
			TEXT("Plugins"), 
			TEXT("Firebase Authentication"),
			LOCTEXT("Firebase Authentication", "Firebase Authentication"), 
			LOCTEXT("Facebook", "Settings for Facebook Authentication"),
			GetMutableDefault<UFirebaseSettings>());
	}
}

void FFirebaseAuthenticationModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(
			TEXT("Project"), 
			TEXT("Plugins"), 
			TEXT("Firebase Authentication"));
	}
}

FFirebaseAuthenticationModule* FFirebaseAuthenticationModule::GetModule()
{
	return FModuleManager::Get().GetModulePtr<FFirebaseAuthenticationModule>("FirebaseAuthentication");
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FFirebaseAuthenticationModule, FirebaseAuthentication)
