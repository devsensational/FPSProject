// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Data/FPCharacterStatData.h"
#include "GameFramework/Character.h"
#include "FPCharacterBase.generated.h"

class AFPWeaponBase;
enum class EFPWeaponType : uint8;

UCLASS(Blueprintable)
class FPSPROJECT_API AFPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* 캐릭터 Collision 섹션 추가 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionRadius = 100.0f;
	
protected:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	void SetCollisionProfile(); //콜리전 Profile 설정

	// 캐릭터 무기 조작 관련 섹션
public:
	void Attack();
	void Reload();

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttack();

	void PerformAttack();
	
	
	// 캐릭터 무기 소유 관련 섹션
public:
	void EquipWeapon(EFPWeaponType InWeaponType);
	void UnequipWeapon();
	void LootWeapon(AFPWeaponBase* InWeapon);
	void DropWeapon(EFPWeaponType InWeaponType);

protected:
	UPROPERTY()
	TMap<EFPWeaponType,TObjectPtr<class AFPWeaponBase>> OwnedWeaponMap; // 현재 소유하고 있는 모든 무기 맵

	UPROPERTY()
	TObjectPtr<AFPWeaponBase> CurrentWeapon; // 현재 손에 들고 있는 무기
	
	UPROPERTY()
	TObjectPtr<AFPWeaponBase> PreviousWeapon; // 이전에 손에 들고 있던 무기

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquip(EFPWeaponType InWeaponType);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnequip();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLootWeapon(int32 InWeaponNetID);
	
	UFUNCTION(Server, Unreliable)
	void ServerDropWeapon(EFPWeaponType InWeaponType); 
	
	void PerformEquip(EFPWeaponType InWeaponType);
	void PerformUnequip();
	void PerformLootWeapon(int32 InWeaponID);
	void PerformDropWeapon(EFPWeaponType InWeaponType);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon(); // 모든 클라이언트에게 무기 장비 적용
	
	//캐릭터 스탯 관련 섹션
public:
	// 데이터 에셋 (서버에서만 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TObjectPtr<UFPCharacterStatData> CharacterStats;

	// 현재 체력 (네트워크 동기화)
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	// 체력 변경 시 클라이언트 반영
	UFUNCTION()
	void OnRep_CurrentHealth();

	// 스탯 변경 함수 (서버에서 실행)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerModifyStat(float HealthDelta, float AttackDelta);
	void ServerModifyStat_Implementation(float HealthDelta, float AttackDelta);
	bool ServerModifyStat_Validate(float HealthDelta, float AttackDelta);
	
public:
	//3인칭 Mesh
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	//TArray<USkeletalMeshComponent*> CharacterMeshes;  // 여러 개의 메쉬를 저장할 배열

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	//USkeletalMeshComponent* ThirdPersonMesh = nullptr;
};