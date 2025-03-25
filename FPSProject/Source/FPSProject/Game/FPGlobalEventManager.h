// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FPGlobalEventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, NewCurrentAmmo, int32, NewCurrentReamingAmmo);

/**
 *	★ 단일 UI에게 값을 전달하거나, 이벤트를 호출하기 위해 만들어진 매니저 클래스
 */
UCLASS()
class FPSPROJECT_API UFPGlobalEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	// 탄약 변경 이벤트 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

};
