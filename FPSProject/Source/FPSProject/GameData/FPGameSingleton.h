// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPWeaponStats.h"
#include "UObject/NoExportTypes.h"
#include "FPGameSingleton.generated.h"

class AFPWeaponBase;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UFPGameSingleton();
	static UFPGameSingleton& Get();

	// 무기 데이터 섹션
public:
	UPROPERTY() // 블루프린트가 아니라 직접 경로에 참조해야 될 듯 
	UDataTable* WeaponDataTable;
	
public:
	FORCEINLINE FPWeaponStats* GetWeaponStat(FName Name) const { return WeaponStatsMap[Name];} // 안전한 검사가 가능하도록 변경해야함
private:
	TMap<FName, FPWeaponStats*> WeaponStatsMap;
};
