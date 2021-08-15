// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseAuthentication.h"
#include "FirebaseAuthenticationSettings.h"
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
			GetMutableDefault<UFirebaseAuthenticationSettings>());
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
