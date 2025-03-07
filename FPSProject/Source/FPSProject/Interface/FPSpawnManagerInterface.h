// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSpawnManagerInterface.generated.h"
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPSpawnManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSPROJECT_API IFPSpawnManagerInterface
{
	GENERATED_BODY()

public:
	virtual void RegisterSpawnPoint(class AFPSpawnPoint* SpawnPoint) = 0;			// 스폰 포인트 등록
	virtual void GetSpawnPointLocation() const = 0;	// 스폰 포인트 위치 가져오기
};
