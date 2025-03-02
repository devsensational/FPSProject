// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MyFPWeaponHitscan.h"
#include "DrawDebugHelpers.h"
#include "FPSProject.h"
#include "Character/FPCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void AMyFPWeaponHitscan::Attack()
{
	Super::Attack();

	LOG_NET(NetworkLog, Log, TEXT("%d:Try Hitscan Attack"), GetUniqueID());
	
	// 플레이어가 조작하는 컨트롤러 가져오기
	APlayerController* PlayerController = Cast<APlayerController>(WeaponOwner->GetController());
	if (!PlayerController) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 트레이스 시작점 (총구 위치) 및 방향 설정
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f); // 10,000cm = 100m

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
		ECC_Visibility, // 충돌 채널 설정 (가시성)
		QueryParams
	);

	if (bHit)
	{
		// 적중한 오브젝트가 있으면 로그 출력
		UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
		
		if (ImpactEffect) // nullptr 체크
		{
			// 피격 위치에 효과 적용
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
		}
	}

	// 디버그 라인 (개발용)
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
#endif
	
}

void AMyFPWeaponHitscan::Reload()
{
	Super::Reload();
}
