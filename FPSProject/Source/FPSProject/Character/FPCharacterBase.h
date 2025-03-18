// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Data/FPCharacterStatData.h"
#include "GameFramework/Character.h"
#include "FPCharacterBase.generated.h"

class UFPCharacterStatComponent;
class AFPGameMode;
class AFPWeaponBase;
enum class EFPWeaponType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, NewCurrentAmmo, int32, NewCurrentReamingAmmo);

UCLASS(Blueprintable)
class FPSPROJECT_API AFPCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	AFPCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* 서버용 로직 섹션 */
	UPROPERTY()
	TObjectPtr<AFPGameMode> GameMode;
	
	virtual void ServerInitializeActor();

	/* 캐릭터 Stat Component 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
	UFPCharacterStatComponent* CharacterStatComponent;

public:
	FORCEINLINE UFPCharacterStatComponent* GetCharacterStatComponent() const { return CharacterStatComponent; }
	
public:
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; 
	
	/* 캐릭터 Collision 섹션 */
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

	/* 히트박스 섹션 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitBoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float HitBoxSize = 100.0f;
	
	//UFUNCTION()
	//void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	void SetCollisionProfile(); //콜리전 Profile 설정

	/* 캐릭터 무기 조작 관련 섹션 */
public:
	void Attack();
	void Reload();

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttack();
	void PerformAttack() const;

	UFUNCTION(Server, Reliable)
	void ServerReload();
	void PerformReload() const;
	
	/* 캐릭터 무기 소유 관련 섹션 */
public:
	void EquipWeapon(EFPWeaponType InWeaponType);
	void UnequipWeapon();
	void LootWeapon(const AFPWeaponBase* InWeapon);
	void DropWeapon(const EFPWeaponType InWeaponType);

	void DropCurrentWeapon();

	FORCEINLINE AFPWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
protected:
	UPROPERTY()
	TMap<EFPWeaponType,TObjectPtr<AFPWeaponBase>> OwnedWeaponMap; // 현재 소유하고 있는 모든 무기 맵

	UPROPERTY()
	TObjectPtr<AFPWeaponBase> CurrentWeapon; // 현재 손에 들고 있는 무기
	
	UPROPERTY()
	TObjectPtr<AFPWeaponBase> PreviousWeapon; // 이전에 손에 들고 있던 무기

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquip(EFPWeaponType InWeaponType);
	
	UFUNCTION(Server, Reliable)
	void ServerUnequip();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLootWeapon(int32 InWeaponNetID);
	
	UFUNCTION(Server, Unreliable)
	void ServerDropWeapon(EFPWeaponType InWeaponType); 

	UFUNCTION(Server, Unreliable)
	void ServerDropCurrentWeapon(); 
	
	void PerformEquip(EFPWeaponType InWeaponType);
	void PerformUnequip();
	void PerformLootWeapon(int32 InWeaponID);
	void PerformDropWeapon(EFPWeaponType InWeaponType);
	
	// 캐릭터 사망 시 멀티캐스트
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnDeath();

	// 캐릭터 사망 시 실제 구현부 (서버-클라이언트 모두 실행됨)
	void PerformDeath();
	
	// 탄약 변경 이벤트
	// 무기가 자주 변경되므로 캐릭터에 델리게이트를 구현, 실제 브로드캐스트는 Weapon에서 일어남
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	FORCEINLINE void BroadcastAmmoChanged(int32 CurrentAmmo, int32 CurrentReamingAmmo) const { OnAmmoChanged.Broadcast(CurrentAmmo, CurrentReamingAmmo); }
	
public:
	//3인칭 Mesh
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	//TArray<USkeletalMeshComponent*> CharacterMeshes;  // 여러 개의 메쉬를 저장할 배열

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	//USkeletalMeshComponent* ThirdPersonMesh = nullptr;
};
