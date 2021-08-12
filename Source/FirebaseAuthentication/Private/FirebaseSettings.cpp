// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseSettings.h"

#if WITH_EDITOR
void UFirebaseSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
#endif
