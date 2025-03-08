// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FPWeaponBase.h"

#include "FPSProject.h"
#include "Character/FPCharacterBase.h"
#include "Game/FPGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPWeaponBase::AFPWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

	// 1인칭용 메시
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);
	FirstPersonMesh->SetOnlyOwnerSee(true); // 소유자만 볼 수 있도록 설정
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// 3인칭용 메시
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);
	ThirdPersonMesh->SetOwnerNoSee(true); // 소유자는 보지 못하도록 설정

	// 콜리전 구 생성 및 설정
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(50.0f); // 반지름 설정
	CollisionSphere->SetHiddenInGame(false); // 게임 중 보이도록 설정
	CollisionSphere->SetGenerateOverlapEvents(true); // 오버랩 이벤트 생성 활성화
	
	SetCollisionProfile(); // 콜리전 프로필 설정
	
}

void AFPWeaponBase::Destroyed()
{
	Super::Destroyed();
	
	// WeaponID TMap에서 제거
	if (HasAuthority())
	{
		if (GameMode)
		{
			GameMode = GetWorld()->GetAuthGameMode<AFPGameMode>();
			GameMode->UnregisterWeaponID(this);
		}
	}
}

void AFPWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPWeaponBase, CurrentAmmo)
}


void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	// WeaponID TMap에 추가 (서버에서만)
	if (HasAuthority())
	{
		GetWorld()->GetAuthGameMode<AFPGameMode>()->RegisterWeaponID(this);
	}
}

void AFPWeaponBase::SetCollisionProfile() const
{
	CollisionSphere->SetCollisionProfileName(TEXT("WeaponPreset"));
}

void AFPWeaponBase::SetSphereCollisionEnabled(bool bEnabled) const
{
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}


// 장비 상호작용(공격, 장비 등) 섹션
// 공격 실행
void AFPWeaponBase::Attack()
{
	// WeaponBase에는 공격이 없음
	// Todo: 하위 클래스에서 실제 공격 로직을 구현할 것(탄약 감소, 총알 발사, 데미지 계산 등)
}

// 현재 탄약이 서버에서 변경 시 클라이언트에게 전파
void AFPWeaponBase::OnRep_ReplicateCurrentAmmo()
{
	LOG_NET(NetworkLog, Log, TEXT("Current Ammo: %d"), CurrentAmmo);
}

void AFPWeaponBase::PlayAnimationAttack()
{
	// WeaponBase에는 애니메이션이 없음
}

void AFPWeaponBase::PlayAnimationReload()
{
	// WeaponBase에는 애니메이션이 없음
}

void AFPWeaponBase::MulticastAttackEffects_Implementation()
{
	// Todo: 공격 이펙트 (파티클, 애니메이션, 사운드) 실행
	UE_LOG(LogTemp, Log, TEXT("Play fire effects on all clients!"));
}

void AFPWeaponBase::Equip(TObjectPtr<AFPCharacterBase> NewOwner)
{
	if (bIsOwned) return;
	
	// Todo: 장비자에 대한 레퍼런스를 설정
	SetOwner(NewOwner);
	bIsOwned = true;
	SetSphereCollisionEnabled(true);
	
	LOG_NET(NetworkLog, Log, TEXT("Set Owner: %d"), NewOwner->GetUniqueID());
}

void AFPWeaponBase::UnEquip()
{
	// Todo: 장비자에 대한 레퍼런스 해제

	SetOwner(nullptr);
	bIsOwned = false;
	SetSphereCollisionEnabled(false);
	
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Owner Removed"));
}

void AFPWeaponBase::Reload()
{
	// Todo: 무기 재장전 실행, 서버 RPC 추가해야됌
	
}
