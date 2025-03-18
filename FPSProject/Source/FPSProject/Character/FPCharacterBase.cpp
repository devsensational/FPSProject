// FPS Project의 캐릭터 베이스 CPP
//


#include "Character/FPCharacterBase.h"

#include "FPSProject.h"
#include "CharacterStat/FPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/FPGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Physics/FPCollision.h"
#include "Weapon/FPWeaponBase.h"

AFPCharacterBase::AFPCharacterBase()
{
	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	// 콜리전 구 생성 및 설정
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(CollisionRadius); // 반지름 설정
	CollisionSphere->SetHiddenInGame(false); // 게임 중 보이도록 설정
	CollisionSphere->SetGenerateOverlapEvents(true); // 오버랩 이벤트 생성 활성화

	HitBoxCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBoxCollision"));
	HitBoxCollision->SetupAttachment(RootComponent);
	HitBoxCollision->SetCapsuleHalfHeight(HitBoxSize);
	HitBoxCollision->SetHiddenInGame(false);
	HitBoxCollision->SetGenerateOverlapEvents(true);
	
	SetCollisionProfile(); // 콜리전 프로필 설정
	
	// 컴포넌트를 생성하고, 기본 소유 액터(캐릭터)에 부착
	CharacterStatComponent = CreateDefaultSubobject<UFPCharacterStatComponent>(TEXT("CharacterStatComponent"));

}

// Unreal Lifecycle
void AFPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())  // 서버에서만 실행
	{
		ServerInitializeActor();
	}
	
}

void AFPCharacterBase::Destroyed()
{
	Super::Destroyed();
	
#if WITH_EDITOR
	if (GIsEditor && !GWorld->IsPlayInEditor())
	{
		return;
	}
#endif
	
	if (HasAuthority())
	{
		GameMode->UnregisterCharacterReference(this);
	}
}

void AFPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPCharacterBase::ServerInitializeActor()
{
	// 오버랩 이벤트 바인딩
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPCharacterBase::OnSphereBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AFPCharacterBase::OnSphereEndOverlap);

	// 게임모드 레퍼런스 바인딩
	GameMode = Cast<AFPGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->RegisterCharacterID(this);
	}
	else
	{
		LOG_NET(NetworkLog, Warning, TEXT("Cannot load Gamemode"));
	}
}

float AFPCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (CharacterStatComponent)
	{
		if (HasAuthority())  // 서버에서만 실행
		{
			CharacterStatComponent->ApplyDamage(DamageAmount);
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called to bind functionality to input
void AFPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AFPCharacterBase::MulticastOnDeath_Implementation()
{
	PerformDeath();
}

void AFPCharacterBase::PerformDeath()
{
	// Todo: UI변경 및 캐릭터 사망 애니메이션 재생, 캐릭터 조작 불가능 하도록 변경
}

// 네트워크 복제 설정
void AFPCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

/* Collision 셋팅 */
void AFPCharacterBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LOG_NET(NetworkLog, Log, TEXT("Weapon Overlap Begin: %s"), *OtherActor->GetName())
	
	// Todo: 오버랩 시작 시 로직
	// 충돌한 액터가 AFPWeaponBase 타입인지 확인
	AFPWeaponBase* OverlappedWeapon = Cast<AFPWeaponBase>(OtherActor);
	if (OverlappedWeapon)
	{
		// AFPWeaponBase 타입의 오브젝트와 충돌했을 때 실행되는 로직
		LOG_NET(NetworkLog, Log, TEXT("Overlap Begin with Weapon: %s"), *OtherActor->GetName());
		
		// Todo: AFPWeaponBase와 충돌 시 로직을 여기에 작성
		
		if(HasAuthority())
		{
			LootWeapon(OverlappedWeapon);
		}
	}
	else
	{
		// AFPWeaponBase 타입이 아닌 다른 오브젝트와 충돌했을 때 실행되는 로직
		LOG_NET(NetworkLog, Log, TEXT("Overlap Begin with other Actor: %s"), *OtherActor->GetName());

		// Todo: AFPWeaponBase 타입이 아닌 다른 오브젝트와 충돌 시 로직을 여기에 작성
	}
}

void AFPCharacterBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	LOG_NET(NetworkLog, Log, TEXT("Weapon Overlap End: %s"), *OtherActor->GetName())
	// Todo: 오버랩 종료 시 로직
}

void AFPCharacterBase::SetCollisionProfile()
{
	CollisionSphere->SetCollisionProfileName(TEXT("WeaponPreset"));
	HitBoxCollision->SetCollisionProfileName(CPROFILE_FPHITBOX);
}


// 장착한 무기 사용
void AFPCharacterBase::Attack()
{
	//LOG_NET(NetworkLog, Log, TEXT("Try Attack"));
	if (HasAuthority()) // 서버라면 직접 공격 실행
	{
		PerformAttack();
	}
	else // 클라이언트라면 서버에게 요청
	{
		ServerAttack();	
	}
}

// 장착한 무기 재장전
void AFPCharacterBase::Reload()
{
	LOG_NET(NetworkLog, Log, TEXT("Try Reload"));
	// ToDo: 서버에서 공격 실행나눠야 함
	if (HasAuthority()) // 서버라면 직접 재장전
	{
		PerformReload();
	}
	else // 클라이언라면 서버에 재장전 요청
	{
		ServerReload();	
	}
}

void AFPCharacterBase::ServerAttack_Implementation()
{
	PerformAttack();
}

bool AFPCharacterBase::ServerAttack_Validate()
{
	return true;
}

void AFPCharacterBase::PerformAttack() const
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
}

void AFPCharacterBase::ServerReload_Implementation()
{
	PerformReload();
}


void AFPCharacterBase::PerformReload() const
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

// 장비 상호작용 및 전투 관련 섹션
// 무기 장비 해제, 장착한 무기에 대한 레퍼런스 해제
void AFPCharacterBase::EquipWeapon(EFPWeaponType InWeaponType)
{
	if (HasAuthority()) // 서버라면 직접 무기 장착
	{
		PerformEquip(InWeaponType);
	}
	else // 클라이언트라면 서버에게 요청
	{
		ServerEquip(InWeaponType);
	}
}

void AFPCharacterBase::UnequipWeapon()
{
	if (HasAuthority()) // 서버라면 직접 장비 해제
	{
		PerformUnequip();
	}
	else // 클라이언트라면 서버에게 요청
	{
		PerformUnequip();
	}
}

// 무기 습득, 습득한 무기에 대한 레퍼런스 설정
void AFPCharacterBase::LootWeapon(const AFPWeaponBase* InWeapon)
{
	int32 WeaponID = InWeapon->GetUniqueID();
	LOG_NET(NetworkLog, Log, TEXT("Loot Weapon Id: %d"), WeaponID)
	if (HasAuthority()) // 서버라면 직접 공격 실행
	{
		PerformLootWeapon(WeaponID);
	}
	else // 클라이언트라면 서버에게 요청
	{
		ServerLootWeapon(WeaponID);
	}
}

// 무기 드랍, 무기를 인벤토리로 부터 레퍼런스를 해제하고 땅에 버림
void AFPCharacterBase::DropWeapon(const EFPWeaponType InWeaponType)
{
	LOG_NET(NetworkLog, Log, TEXT("Try drop Weapon Id: %d"), InWeaponType)
	if (HasAuthority()) // 서버라면 직접 공격 실행
	{
		PerformDropWeapon(InWeaponType);
	}
	else // 클라이언트라면 서버에게 요청
	{
		ServerDropWeapon(InWeaponType);
	}
}

// 현재 손에 들고있는 무기 드랍
void AFPCharacterBase::DropCurrentWeapon()
{
	LOG_NET(NetworkLog, Log, TEXT("Try drop current Weapon"))
	if (HasAuthority()) // 서버라면 직접 공격 실행
	{
		if (CurrentWeapon)
		{
			PerformDropWeapon(CurrentWeapon->GetType());
		}
	}
	else // 클라이언트라면 서버에게 요청
	{
		ServerDropCurrentWeapon();
	}
	
}

void AFPCharacterBase::ServerDropCurrentWeapon_Implementation()
{
	if (CurrentWeapon)
	{
		PerformDropWeapon(CurrentWeapon->GetType());
	}
}

// 서버에서 장비 장착
// 무기 장비, 장착한 무기에 대한 레퍼런스 설정
void AFPCharacterBase::ServerEquip_Implementation(const EFPWeaponType InWeaponType)
{
	PerformEquip(InWeaponType);
}

bool AFPCharacterBase::ServerEquip_Validate(const EFPWeaponType InWeaponType)
{
	return true;
}

// 서버에서 장비 해제
void AFPCharacterBase::ServerUnequip_Implementation()
{
	PerformUnequip();
}

// 서버에서 장비 습득
void AFPCharacterBase::ServerLootWeapon_Implementation(int32 InWeaponNetID)
{
	// 서버에서만 실행, 캐릭터가 장비에 대한 레퍼런스를 설정하는 함수 호출 
	PerformLootWeapon(InWeaponNetID);
	// 이 함수에서 장비 레퍼런스를 직접 찾아 PerformLoot으로 실행할건지 아닌지 고민해봐야함
	// 이 함수에서 장비레퍼런스를 찾아야 제대로된 유효성 검사가 될 것 같긴 함
}

bool AFPCharacterBase::ServerLootWeapon_Validate(int32 InWeaponNetID)
{
	// Todo: 캐릭터와 장비 사이의 거리를 측정하여 유효성을 검사할 것 
	return true;
}

// 서버에서 장비 버리기
void AFPCharacterBase::ServerDropWeapon_Implementation(EFPWeaponType InWeaponType)
{
	//PerformDropWeapon();
}

// 실제 서버에서 작동되는 구현부
// 무기를 손에 들기(장비)
void AFPCharacterBase::PerformEquip(EFPWeaponType InWeaponType)
{
	TObjectPtr<AFPWeaponBase> NextWeapon;
	if (OwnedWeaponMap.Contains(InWeaponType))
	{
		NextWeapon = OwnedWeaponMap[InWeaponType];
		
		LOG_NET(NetworkLog, Log, TEXT("Weapon find: %d"), (int32)InWeaponType);
	}
	
	// 캐릭터가 현재 손에 들고 있는 장비에 대한 레퍼런스 설정
	if (CurrentWeapon && CurrentWeapon->GetType() != InWeaponType)
	{
		PreviousWeapon = CurrentWeapon;
		CurrentWeapon = NextWeapon;
	}
	else
	{
		CurrentWeapon = NextWeapon;
	}

	CurrentWeapon->BindReference(this);

	
	FName WeaponSocket(TEXT("hand_socket"));
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	LOG_NET(NetworkLog, Log, TEXT("Equip Weapon: %d"), (int32)InWeaponType);
}

// 무기를 손에서 해제하기(장비는 소유한 상태)
void AFPCharacterBase::PerformUnequip()
{
	
}

// 무기를 바닥에서 줍기
void AFPCharacterBase::PerformLootWeapon(int32 LootedWeapon)
{
	// 아이템을 인벤토리 MAP에 추가
	// ToDo: 레퍼런스 참조 최적화 필요??
	AFPWeaponBase* NextWeapon = GetWorld()->GetAuthGameMode<AFPGameMode>()->GetWeaponByID(LootedWeapon);
	if (NextWeapon->bIsOwned) return; // 이미 소유자가 있으면 줍지 않음
	if (NextWeapon)
	{
		OwnedWeaponMap.Add(NextWeapon->GetType(), NextWeapon);
		NextWeapon->BindReference(this);
		LOG_NET(NetworkLog, Log, TEXT("Weapon(%d) is registered Character(%d)"), LootedWeapon, GetUniqueID());
	}

	PerformEquip(NextWeapon->GetType()); // 테스트 용, 나중에 제거해야됨
	//Todo: 아이템 주운 것에 대한 UI처리 필요
}

// 무기를 바닥에 버리기
void AFPCharacterBase::PerformDropWeapon(EFPWeaponType InWeaponType)
{
	//무기를 땅에 버리고, 레퍼런스를 해제. 손에 들고 있는 무기를 버릴 경우 이전 무기를 손에 장착하고 땅에 버리기
	CurrentWeapon->UnbindReference();
	
	if (CurrentWeapon->GetType() != InWeaponType)
	{
		if (PreviousWeapon != nullptr) PerformEquip(PreviousWeapon->GetType());
	}
	else
	{
		CurrentWeapon = nullptr;
	}
	OwnedWeaponMap.Remove(InWeaponType);
	LOG_NET(NetworkLog, Log, TEXT("Weapon dropped(unbind): %d"), InWeaponType);
}
