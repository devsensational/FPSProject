// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPDirectionIndicator.h"

#include "EngineUtils.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UFPDirectionIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TargetActor.IsValid() || !ArrowImage) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC || !PC->GetPawn()) return;

	FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	// 방향 벡터 계산
	FVector Direction = (TargetLocation - PlayerLocation).GetSafeNormal();

	// 카메라 기준 회전 보정
	FRotator CamRot = PC->PlayerCameraManager->GetCameraRotation();
	float TargetYaw = Direction.Rotation().Yaw;
	float RelativeYaw = TargetYaw - CamRot.Yaw;

	// 위젯 이미지 회전
	ArrowImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f)); // 중심 기준 회전
	ArrowImage->SetRenderTransformAngle(RelativeYaw);
}

void UFPDirectionIndicator::SetTargetLocation(AActor* InTargetActor)
{
	TargetActor = InTargetActor;
}
