// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/FPCharacterStatComponent.h"

#include "FPSProject.h"
#include "Character/Data/FPCharacterStatData.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UFPCharacterStatComponent::UFPCharacterStatComponent()
{
    SetIsReplicatedByDefault(true);
}

void UFPCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFPCharacterStatComponent, CurrentHealth);
}

// Called when the game starts
void UFPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// 체력이 변경될 때 실행 (클라이언트에서 업데이트)
void UFPCharacterStatComponent::OnRep_CurrentHealth()
{
	UE_LOG(LogTemp, Log, TEXT("CurrentHealth changed"));

	//ToDo: UI 업데이트 필요 시 여기서 처리
	OnHealthChanged.Broadcast(CurrentHealth); // 체력 변경 이벤트 브로드캐스트
}

// 서버에서만 실행되는 스탯 변경 함수
void UFPCharacterStatComponent::ServerModifyStat_Implementation(float HealthDelta, float AttackDelta)
{
	if (GetOwner() && !GetOwner()->HasAuthority())  // 서버에서만 실행
	{
		CurrentHealth += HealthDelta;

	}
}

// 유효성 검사 (너무 큰 값 제한)
bool UFPCharacterStatComponent::ServerModifyStat_Validate(float HealthDelta, float AttackDelta)
{
	return FMath::Abs(HealthDelta) < 1000.0f && FMath::Abs(AttackDelta) < 1000.0f;
}


void UFPCharacterStatComponent::Client_UpdateHUD_Implementation()
{
	
}

float UFPCharacterStatComponent::ApplyDamage(float DamageAmount)
{
	UE_LOG(LogTemp, Log, TEXT("Character %s took %f damage, Remaining Health: %f"), *GetName(), DamageAmount, CurrentHealth - DamageAmount);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	if (CurrentHealth <= 0)
	{
		// ToDo: 사망 처리
		//PerformDeath();
		UE_LOG(LogTemp, Log, TEXT("%s has died"), *GetName());
	}

	OnRep_CurrentHealth(); // 서버에게도 명시
	return DamageAmount;
}
