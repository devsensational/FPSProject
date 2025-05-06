// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FPWeaponBase.h"

#include "FPSProject.h"
#include "Character/FPCharacterBase.h"
#include "Component/FPInteractableComponent.h"
#include "Game/FPGameInstance.h"
#include "Game/FPGameMode.h"
#include "GameData/FPWeaponStats.h"
#include "Net/UnrealNetwork.h"
#include "Physics/FPCollision.h"

// Sets default values
AFPWeaponBase::AFPWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

	// 3인칭용 메시
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));

	ThirdPersonMesh->SetupAttachment(RootComponent);
	ThirdPersonMesh->SetOwnerNoSee(true); // 소유자는 보지 못하도록 설정

	// 콜리전 구 생성 및 설정
	//CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	//CollisionSphere->SetupAttachment(ThirdPersonMesh);
	//CollisionSphere->SetSphereRadius(50.0f); // 반지름 설정
	//CollisionSphere->SetHiddenInGame(false); // 게임 중 보이도록 설정
	//CollisionSphere->SetGenerateOverlapEvents(true); // 오버랩 이벤트 생성 활성화

	// 키보드 상호작용 섹션
	InteractableComponent = CreateDefaultSubobject<UFPInteractableComponent>(TEXT("InteractBox"));
	InteractableComponent->SetupAttachment(RootComponent);
	InteractableComponent->SetHiddenInGame(false); // 게임 중 보이도록 설정
	InteractableComponent->SetGenerateOverlapEvents(true);
	
	//
	PrimaryActorTick.bCanEverTick = false;
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
	DOREPLIFETIME(AFPWeaponBase, CurrentRemainingAmmo)
}


void AFPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	// WeaponID TMap에 추가 (서버에서만)
	if (HasAuthority())
	{
		GetWorld()->GetAuthGameMode<AFPGameMode>()->RegisterWeaponID(this);
	}
	
	// GameInstance 가져오기
	if (UFPGameInstance* GameInstance = Cast<UFPGameInstance>(GetWorld()->GetGameInstance()))
	{
		FPWeaponStats WeaponStats;
		if (GameInstance->GetWeaponStatsByName(Name, WeaponStats))
		{
			// 데이터 적용
			Type					= WeaponStats.Type;
			Damage					= WeaponStats.Damage;
			Accuracy				= WeaponStats.Accuracy;
			RPM						= WeaponStats.RPM;
			Price					= WeaponStats.Price;
			ReloadTime				= WeaponStats.ReloadTime;
			MaxAmmo					= WeaponStats.MaxAmmo;
			MaxRemainingAmmo		= WeaponStats.MaxRemainingAmmo;
			CurrentAmmo				= MaxAmmo;
			CurrentRemainingAmmo	= MaxRemainingAmmo;

			UE_LOG(LogTemp, Log, TEXT("Weapon stats loaded: %s (Damage: %f, Accuracy: %f)"), *Name.ToString(), Damage, Accuracy);
		}
	}
	else
	{
			UE_LOG(LogTemp, Warning, TEXT("Weapon stats not loaded: %s"), *Name.ToString());
	}
}

void AFPWeaponBase::SetSphereCollisionEnabled(bool bEnabled) const
{
	if (InteractableComponent)
	{
		InteractableComponent->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
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

	if (const auto FPCharacter = Cast<AFPCharacterBase>(Owner))
	{
		FPCharacter->BroadcastAmmoChanged(CurrentAmmo, CurrentRemainingAmmo);
	}
}

void AFPWeaponBase::PlayAnimationAttack()
{
	// WeaponBase에는 애니메이션이 없음
	// Todo: 1인칭용 애니메이션 컴포넌트를 구현하여 이벤트를 전달하는 식으로 구현
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

void AFPWeaponBase::BindReference(TObjectPtr<AFPCharacterBase> NewOwner)
{
	if (bIsOwned) return;
	
	// 장비자에 대한 레퍼런스를 설정
	SetOwner(NewOwner);
	WeaponOwner = NewOwner;
	SetLootable(true);

	MulticastBindWeapon();
	
	LOG_NET(NetworkLog, Log, TEXT("Set Owner: %d"), NewOwner->GetUniqueID());
}

void AFPWeaponBase::UnbindReference()
{
	SetActorLocation(Owner->GetActorLocation());
	
	SetOwner(nullptr); //장비자에 대한 레퍼런스 해제
	WeaponOwner = nullptr;

	GetWorldTimerManager().SetTimer(
	TimerHandle,
	[this]() { SetLootable(false); },
	LootableCooldown,       // 지연시간 (초 단위)
	false       // 반복 여부 (true면 반복 실행)
	);
	
	MulticastDropWeapon();
	
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Owner Removed"));
}

void AFPWeaponBase::SetLootable(bool bValue)
{
	bIsOwned = bValue;
	SetSphereCollisionEnabled(!bValue);
}

void AFPWeaponBase::MulticastBindWeapon_Implementation()
{
	// Weapon Owner는 3인칭을 표시하지 않음
	if (WeaponOwner && WeaponOwner->IsLocallyControlled())
	{
		ThirdPersonMesh->SetVisibility(false);
	}
	else
	{
		ThirdPersonMesh->SetVisibility(true);
	}
}

void AFPWeaponBase::MulticastUnbindWeapon_Implementation()
{
	// 장비는 소유하되 장비를 해제할 시 모든 사람에게 3인칭 표시 안함
	ThirdPersonMesh->SetVisibility(false);
	
}

void AFPWeaponBase::MulticastDropWeapon_Implementation()
{
	// 장비 드랍 시 모든 사람에게 3인칭을 표시
	ThirdPersonMesh->SetVisibility(true);
	
}

void AFPWeaponBase::Reload()
{
	if (CurrentRemainingAmmo <= 0) return;
	
	GetWorldTimerManager().SetTimer(
	TimerHandle,
	this,
	&AFPWeaponBase::ExecuteReload,
	ReloadTime,       // 지연시간 (초 단위)
	false       // 반복 여부 (true면 반복 실행)
	);
}

void AFPWeaponBase::ExecuteReload()
{
	// 타이머 종료 후 무기 재장전 실행
	int32 ReloadAmmoCount = MaxAmmo - CurrentAmmo;
	CurrentAmmo = FMath::Clamp(CurrentAmmo + CurrentRemainingAmmo, 0, MaxAmmo);
	CurrentRemainingAmmo = FMath::Clamp( CurrentRemainingAmmo - ReloadAmmoCount, 0, MaxRemainingAmmo);
	
}

void AFPWeaponBase::Interact_Implementation(AFPCharacterBase* Interactor)
{
	if (!Interactor) return;

	// 상호작용을 수행한 액터에게 이동함으로써 아이템 줍기 실행
	FVector TargetLocation = Interactor->GetActorLocation();
	SetActorLocation(TargetLocation);
	
	UE_LOG(LogTemp, Log, TEXT("아이템이 줍혔습니다."));
	
}