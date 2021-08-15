// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseAuthenticationSettings.h"

#if WITH_EDITOR
void UFirebaseAuthenticationSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
#endif
