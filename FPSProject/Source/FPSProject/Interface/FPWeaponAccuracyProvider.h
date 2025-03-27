// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPWeaponAccuracyProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPWeaponAccuracyProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Crosshair UI의 십자선 분산도를 결정하기 위한 총기의 명중률을 갖고오는 인터페이스
 */
class FPSPROJECT_API IFPWeaponAccuracyProvider
{
	GENERATED_BODY()

public:
	virtual float GetCurrentWeaponAccuracy() = 0;
};
