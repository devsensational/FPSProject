// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FPCharacterStatData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FPSPROJECT_API UFPCharacterStatData : public UDataAsset
{
	GENERATED_BODY()

	// 캐릭터의 기본 스탯
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxArmor = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Speed = 600.0f;
};
