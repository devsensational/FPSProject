// Weapon에 대한 전반적인 내용을 구현하는 클래스
// Server를 통해 무기가 상호작용되거나, 소유자에 대한 레퍼런스를 설정하는 내용 구현

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Game/FPGameMode.h"

class AFPCharacterBase;

#include "FPWeaponBase.generated.h"


UENUM()
enum class EFPWeaponType : uint8
{
	WT_None UMETA(DisplayName = "None"),
	WT_Primary UMETA(DisplayName = "Primary"),
	WT_Secondary UMETA(DisplayName = "Secondary"),
	WT_Melee UMETA(DisplayName = "Melee"),
	WT_Throwable UMETA(DisplayName = "Throwable"),
};

UENUM()
enum class EFPWeaponFireType : uint8
{
	WFT_None UMETA(DisplayName = "None"),
	WFT_Auto UMETA(DisplayName = "Auto"),
	WFT_Semi UMETA(DisplayName = "Semi"),
	WFT_Burst UMETA(DisplayName = "Burst"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, NewCurrentAmmo, int32, NewCurrentReamingAmmo);

UCLASS(Blueprintable)
class FPSPROJECT_API AFPWeaponBase : public AActor
{
	GENERATED_BODY()

	/* 초기화 및 Lifecycle 섹션 */
public:	
	AFPWeaponBase();
	virtual void Destroyed() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	
	TObjectPtr<AFPGameMode> GameMode;
	
	/* 무기 Collision 섹션 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;
	
private:
	void SetCollisionProfile() const; //콜리전 Profile 설정
	void SetSphereCollisionEnabled(bool bEnabled) const; // 콜리전 On/Off 설정
	
	
	/* 무기 오너 관련 섹션 */
protected:
	//UPROPERTY()
	//TObjectPtr<AFPCharacterBase> WeaponOwner = nullptr; // 무기 소유자 레퍼런스

public:
	UPROPERTY()
	uint8 bIsOwned = false;

	
	/* 무기 상호작용 관련 섹션 */
	// ToDO: 무기를 사용하는 것은 모두 서버로 부터 호출되며 완료된 것을 브로드캐스트를 통해 서버로 전달해야 함 
public:
	virtual void Attack();					// 무기를 사용해서 공격
	virtual void Equip(TObjectPtr<AFPCharacterBase> NewOwner);	// 무기를 장비, 무기 소유자에 대한 레퍼런스 설정
	virtual void UnEquip();					// 무기 장비 해제, 무기 소유자의 레퍼런스 해제
	virtual void Reload();
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackEffects(); // 모든 클라이언트에게 공격 이펙트 적용
	
	/* 무기 Stats 관련 섹션 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UDataTable> DataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName Name; //WeaponName을 통해 테이블 접근

	EFPWeaponType GetType() const { return Type; }
	float GetBeforeFireTime() const { return BeforeFireTime; }
	
	// 탄약 변경 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;
	
protected:
	EFPWeaponType Type			= EFPWeaponType::WT_None;
	float Damage				= 30.0f;
	float Accuracy				= 30.0f;
	float Magazine				= 30.0f;
	float RPM					= 300.0f;
	int32 Price					= 2700;
	int32 MaxAmmo				= 500;
	int32 MaxRemainingAmmo		= 120;

	UPROPERTY(ReplicatedUsing=OnRep_ReplicateCurrentAmmo)
	int32 CurrentAmmo			= MaxAmmo;
	int32 CurrentRemainingAmmo	= MaxRemainingAmmo;

	// 클라이언트에서 서버로 보내는 명령의 수를 제한함과 동시에,
	// 서버에서 무기의 공격 시간을 체크함으로써 치트를 사용하여 총의 연사속도를 변경하는 것을 막기 위한 설계
	float FireDelay = 0;		// 총기 발사 간격 내부 쿨타임(클라이언트)
	float BeforeFireTime = 0;	// 총기 발사 시 서버에서 현재 시간과 이전 시간을 비교하기 위한 변수
	float CurrentFireTime = 0;
	
	// 무기 Mesh 관련 변수
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* ThirdPersonMesh;

protected:
	UFUNCTION()
	virtual void OnRep_ReplicateCurrentAmmo();
	
	// 무기 Animation 섹션
protected:
	virtual void PlayAnimationAttack();
	virtual void PlayAnimationReload();
};
