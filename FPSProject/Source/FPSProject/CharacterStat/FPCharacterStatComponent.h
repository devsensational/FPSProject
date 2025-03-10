// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPCharacterStatComponent.generated.h"

class UFPCharacterStatData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UFPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/* Unreal Lifecycle 섹션 */
	UFPCharacterStatComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	/* 캐릭터 스탯 관련 섹션 */
public:
	// 최대 체력
	float MaxHealth = 100;
	
	// 체력 변경 이벤트 (캐릭터 또는 컨트롤러에서 바인딩하여 사용)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;
	
	// 현재 체력 (네트워크 동기화)
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentHealth, VisibleInstanceOnly, BlueprintReadOnly, Category = "Stat")
	float CurrentHealth;

	// 체력 변경 시 클라이언트 반영
	UFUNCTION()
	void OnRep_CurrentHealth();

	// 스탯 변경 함수 (서버에서 실행)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerModifyStat(float HealthDelta, float AttackDelta);

	// 클라이언트 전용 HUD 업데이트 함수
	UFUNCTION(Client, Reliable)
	void Client_UpdateHUD();
	void Client_UpdateHUD_Implementation();
	
	/* 캐릭터 피격 판정 섹션 */
public:
	float ApplyDamage(float DamageAmount); 
};
