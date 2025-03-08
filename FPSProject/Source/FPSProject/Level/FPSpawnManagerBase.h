// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/FPSpawnManagerInterface.h"
#include "FPSpawnManagerBase.generated.h"

UCLASS(Blueprintable)
class FPSPROJECT_API AFPSpawnManagerBase : public AActor, public IFPSpawnManagerInterface
{
	GENERATED_BODY()
	
public:	
	AFPSpawnManagerBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	/* 스폰 포인트 섹션 */
protected:
	UPROPERTY()
	TArray<TObjectPtr<AFPSpawnPoint>> SpawnPointList;

public:
	virtual void RegisterSpawnPoint(AFPSpawnPoint* SpawnPoint) override;			// 스폰 포인트 등록
	virtual void GetSpawnPointLocation() const override;	// 스폰 포인트 위치 가져오기
	
};
