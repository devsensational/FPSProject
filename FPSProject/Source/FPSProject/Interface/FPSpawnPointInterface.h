// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSpawnPointInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPSpawnPointInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSPROJECT_API IFPSpawnPointInterface
{
	GENERATED_BODY()

	// 이 클래스에 인터페이스 함수를 추가합니다. 이것은 이 인터페이스를 구현하기 위해 상속될 클래스입니다.
public:
	virtual FVector GetSpawnLocation() const = 0;
	virtual FRotator GetSpawnRotation() const = 0;
	
};
