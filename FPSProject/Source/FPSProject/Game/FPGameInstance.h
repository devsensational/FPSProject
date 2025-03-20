// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPGameInstance.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API UFPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 데이터 테이블 레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	TObjectPtr<UDataTable> WeaponDataTable;

	// 특정 무기 정보를 이름(FName)으로 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon Data")
	bool GetWeaponStatsByName(FName WeaponName, FPWeaponStats& OutWeaponStats) const;
};
