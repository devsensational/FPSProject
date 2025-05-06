// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Weapon/FPWeaponBase.h"
#include "FPCharacterPlayer.generated.h"


class UCameraComponent;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPCharacterPlayer : public AFPCharacterBase
{
	GENERATED_BODY()

public:
	AFPCharacterPlayer();

	virtual void Tick(float DeltaSeconds) override;
	
	/* 언리얼 라이프사이클 */
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 네트워크 복제를 위한 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 팀 설정
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
	int32 TeamID;

	// 로컬 플레이어인지 확인
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsLocalPlayer;

	// 1인칭 Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;

	/* Input Section */
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipPrimaryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipSecondaryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipMeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipThrowableAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DropCurrentWeaponAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IntercatAction;

	/* Wrapper */
	FORCEINLINE void EquipPrimaryWeaponWrapper()	{ EquipWeapon(EFPWeaponType::WT_Primary); }
	FORCEINLINE void EquipSecondaryWeaponWrapper()	{ EquipWeapon(EFPWeaponType::WT_Secondary); }
	FORCEINLINE void EquipMeleeWeaponWrapper()		{ EquipWeapon(EFPWeaponType::WT_Melee); }
	
	// 입력 처리 함수
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void virtual Jump() override;

	/* 상호작용 섹션 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	float InteractRange = 100.f;
	
protected:
	void TryInteract();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTryInteract();
	void PerformTryInteract();

private:
	UPROPERTY()
	TObjectPtr<UFPInteractableComponent> LastHighlightedComp;
};
