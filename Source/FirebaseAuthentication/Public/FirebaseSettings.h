/**
 *	Copyright (C) 2021. Nikita Klimov. All rights reserved.
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the Apache License Version 2.0.
 * 
 *	See file LICENSE.md for further informations on licensing terms.
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FirebaseSettings.generated.h"

UCLASS(transient, config = Engine)
class UFirebaseSettings : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Facebook")
	FString FacebookAppID;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};