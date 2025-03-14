// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FPWeaponHitscan.h"
#include "DrawDebugHelpers.h"
#include "FPSProject.h"
#include "Character/FPCharacterBase.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/FPCollision.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void AFPWeaponHitscan::Attack()
{
	Super::Attack();
	
	/* 기본 공격 수행 로직 */
	if (CurrentAmmo <= 0) return; // 총알이 없을 시 실행안함

	CurrentFireTime = GetWorld()->GetTimeSeconds();
	// GetTimeSeconds()는 45일동안 같은 레벨에서 실행될 경우 부동소수점 문제가 발생할 수 있음
	// 따라서 MMORPG처럼 시간 제한이 없는 게임에서는 GetRealTimeSeconds()를 고려해볼 수 잇음
	
	if (CurrentFireTime - BeforeFireTime < 1 / (RPM / 60.0f)) return; // 서버에서 발사 속도 제한

	BeforeFireTime = CurrentFireTime;
	CurrentAmmo = FMath::Clamp(CurrentAmmo - 1, 0, MaxAmmo);

	if (HasAuthority()) OnRep_ReplicateCurrentAmmo(); //서버의 경우
	
	HitScanAttack();
	LOG_NET(NetworkLog, Log, TEXT("%d:Hitscan Attack"), GetUniqueID());
}

void AFPWeaponHitscan::Reload()
{
	Super::Reload();
}

//히트스캔방식 공격 구현
void AFPWeaponHitscan::HitScanAttack()
{
	// 플레이어가 조작하는 컨트롤러 가져오기
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PlayerController) return;

	FVector CameraLocation;		
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 정확도에 따른 랜덤 오차 적용
	float RandomX = FMath::FRandRange(-Accuracy, Accuracy);
	float RandomY = FMath::FRandRange(-Accuracy, Accuracy);

	FVector RightVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);
	FVector UpVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Z);
	
	
	// 트레이스 시작점 (총구 위치) 및 방향 설정
	FVector TraceStart = CameraLocation;
	//FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f); // 10,000cm = 100m
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f) + (RightVector * RandomX) + (UpVector * RandomY);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 무기 자체는 무시
	QueryParams.AddIgnoredActor(GetOwner()); // 플레이어도 무시
	QueryParams.bReturnPhysicalMaterial = true; // 물리 재질 정보 반환 (피드백용)

	// 트레이스 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		CCHANNEL_FPHITSCAN, // 충돌 채널 설정
		QueryParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitActor->GetName());

			AFPCharacterBase* HitCharacter = Cast<AFPCharacterBase>(HitActor);
			if (HitCharacter)
			{
				// 피해 정보를 담은 FPointDamageEvent 생성
				FPointDamageEvent DamageEvent;
				DamageEvent.Damage = Damage;
				DamageEvent.HitInfo = HitResult;
				DamageEvent.ShotDirection = CameraRotation.Vector();
				DamageEvent.DamageTypeClass = UDamageType::StaticClass();
				
				HitCharacter->TakeDamage(Damage, FDamageEvent(), PlayerController, this);
				
				UE_LOG(LogTemp, Log, TEXT("[Weapon] %s dealt %f damage to %s"), 
								*GetOwner()->GetName(), Damage, *HitCharacter->GetName());
			}
			
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
			}
		}
	}

	// 디버그 라인 (개발용)
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
#endif
	
}
