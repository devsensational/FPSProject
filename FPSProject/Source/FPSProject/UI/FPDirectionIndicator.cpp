// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPDirectionIndicator.h"

#include "EngineUtils.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UFPDirectionIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TargetActor.IsValid() || !ArrowImage) return;

	if (!PlayerController || !PlayerController->GetPawn()) return;

	FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	// 방향 벡터 계산
	FVector Direction = (TargetLocation - PlayerLocation).GetSafeNormal();

	// 카메라 기준 회전 보정
	FRotator CamRot = PlayerController->PlayerCameraManager->GetCameraRotation();
	float TargetYaw = Direction.Rotation().Yaw;
	float RelativeYaw = TargetYaw - CamRot.Yaw;

	// 위젯 이미지 회전
	ArrowImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f)); // 중심 기준 회전
	ArrowImage->SetRenderTransformAngle(RelativeYaw);
}

void UFPDirectionIndicator::SetTargetLocation(AActor* InTargetActor)
{
	TargetActor = InTargetActor;
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

//Todo: 서서히 사라지는 연출과, Set 발동 시 생기는 연출 필요