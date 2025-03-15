// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "FPCharacterPlayer.generated.h"


/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPCharacterPlayer : public AFPCharacterBase
{
	GENERATED_BODY()

public:
	AFPCharacterPlayer();
	
	// 팀 설정
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
	int32 TeamID;

	// 로컬 플레이어인지 확인
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsLocalPlayer;

	// 1인칭 Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 네트워크 복제를 위한 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 입력 처리 함수
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void virtual Jump() override;
};
