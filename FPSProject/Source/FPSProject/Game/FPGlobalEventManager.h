// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FPGlobalEventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, NewCurrentAmmo, int32, NewCurrentReamingAmmo);
DECLARE_DELEGATE_OneParam(FOnMovementModeChanged, EMovementMode);
DECLARE_DELEGATE_OneParam(FSetTargetLocation, AActor*)

/**
 *	★ 단일 UI에게 값을 전달하거나, 이벤트를 호출하기 위해 만들어진 매니저 클래스
 */
UCLASS()
class FPSPROJECT_API UFPGlobalEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	// 체력 변경 이벤트 (캐릭터 또는 컨트롤러에서 바인딩하여 사용)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;
	
	// 탄약 변경 이벤트 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	// 움직임 변경 시 총기 명중률을 보정하는 델리게이트
	FOnMovementModeChanged OnMovementModeChanged;
	
	// 방향 지시 UI의 Target을 설정하는 델리게이트
	FSetTargetLocation SetTargetLocation;
};
