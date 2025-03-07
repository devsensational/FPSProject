// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Interface/FPSpawnManagerInterface.h"
#include "FPSpawnPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API AFPSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	AFPSpawnPoint();
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/* 스폰 포인트 데이터 섹션 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnNumber;
	
	/* 스폰 포인트 지정 관련 섹션 */
private:
	TScriptInterface<IFPSpawnManagerInterface> SpawnManager;
	
	void Init();
};
