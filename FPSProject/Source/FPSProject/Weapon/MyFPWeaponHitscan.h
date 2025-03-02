// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/FPWeaponBase.h"
#include "MyFPWeaponHitscan.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API AMyFPWeaponHitscan : public AFPWeaponBase
{
	GENERATED_BODY()

	
	/* 무기 작동 관련 섹션 */
	// ToDO: 무기를 사용하는 것은 모두 서버로 부터 호출되며 완료된 것을 브로드캐스트를 통해 서버로 전달해야 함 
public:
	virtual void Attack() override;					// 무기를 사용해서 공격
	virtual void Reload() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactEffect;
};
