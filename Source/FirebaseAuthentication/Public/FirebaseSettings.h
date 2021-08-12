// Copyright (C) 2021. Nikita Klimov. All rights reserved.

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
