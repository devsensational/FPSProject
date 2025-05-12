// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPCharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPSProject.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Component/FPInteractableComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Physics/FPCollision.h"
#include "Weapon/FPWeaponBase.h"

//생성자
AFPCharacterPlayer::AFPCharacterPlayer()
{// 1인칭 카메라 설정
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(GetMesh()); // 본체(3인칭 캐릭터)와 연결
	FirstPersonMesh->SetOnlyOwnerSee(true); // 본인만 볼 수 있도록 설정
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// 3인칭 캐릭터 설정
	//ThirdPersonMesh = CharacterMeshes; // 캐릭터 여러개로 만들 시 수정 해야 함
	GetMesh()->SetOwnerNoSee(true);

	//상호작용 섹션
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh());
	FollowCamera->bUsePawnControlRotation = true;
	
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AFPCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/* 상호작용을 위한 Ray 발사 섹션 */
	if (IsLocallyControlled())
	{
		FVector Start = FollowCamera->GetComponentLocation();
		FVector End = Start + FollowCamera->GetForwardVector() * InteractRange;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, CCHANNEL_FPINTERACT, Params))
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				UFPInteractableComponent* InteractableComp = HitActor->FindComponentByClass<UFPInteractableComponent>();

				if (InteractableComp)
				{
					// 이전 대상과 다르면 전 대상은 끄기
					if (LastHighlightedComp && LastHighlightedComp != InteractableComp)
					{
						LastHighlightedComp->Highlight(false);
					}

					// 새 대상 Highlight 켜기
					InteractableComp->Highlight(true);
					LastHighlightedComp = InteractableComp;
					return;
				}
			}
		}

		// 부딪힌게 없거나 상호작용 대상이 아니면
		if (LastHighlightedComp)
		{
			LastHighlightedComp->Highlight(false);
			LastHighlightedComp = nullptr;
		}
	}
}

void AFPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 로컬 플레이어인지 확인
	if (IsLocallyControlled())
	{
		bIsLocalPlayer = true;

		// 로컬 플레이어만 입력 매핑 컨텍스트 추가
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}
	else
	{
		// 원격 플레이어에 대한 처리
		bIsLocalPlayer = false;
	}

	// 서버에서만 실행되어야 하는 로직
	if (HasAuthority())
	{
		// 서버 전용 초기화 코드
	}
}

void AFPCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// TeamID와 bIsLocalPlayer를 네트워크 복제
	DOREPLIFETIME(AFPCharacterPlayer, TeamID);
	DOREPLIFETIME(AFPCharacterPlayer, bIsLocalPlayer);
}

/* 입력 관련 섹션 */
//캐릭터 이동
void AFPCharacterPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

//카메라 회전
void AFPCharacterPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// 점프
void AFPCharacterPlayer::Jump()
{
	Super::Jump();
}

// 상호작용
void AFPCharacterPlayer::TryInteract()
{
	if (HasAuthority()) // 서버라면 직접 상호작용
	{
		PerformTryInteract();
	}
	else // 클라이언트라면 서버에게 요청
	{
		ServerTryInteract();
	}
	
}

void AFPCharacterPlayer::ServerTryInteract_Implementation()
{
	PerformTryInteract();
}

bool AFPCharacterPlayer::ServerTryInteract_Validate()
{
	return true;
}

void AFPCharacterPlayer::PerformTryInteract()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * InteractRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Hit된 컴포넌트가 FPInteractableComponent 인지 확인
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, CCHANNEL_FPINTERACT, Params))
	{
		LOG_NET(NetworkLog, Warning, TEXT("Try Interact"));

		// 부딪힌 컴포넌트가 FPInteractableComponent인지 확인
		if (UFPInteractableComponent* InteractableComp = Cast<UFPInteractableComponent>(Hit.Component.Get()))
		{
			// 상호작용 실행
			InteractableComp->Interact(this);
		}
	}
	// 디버그 라인 (개발용)
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
#endif
}

// Input binding
void AFPCharacterPlayer::SetupPlayerInputComponent(UInputComponent*	PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::Look);
		
		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::Attack);

		//Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::Reload);

		//주무기 장착
		EnhancedInputComponent->BindAction(EquipPrimaryAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::EquipPrimaryWeaponWrapper);

		//보조무기 장착
		EnhancedInputComponent->BindAction(EquipSecondaryAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::EquipSecondaryWeaponWrapper);

		//근접무기 장착
		EnhancedInputComponent->BindAction(EquipMeleeAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::EquipMeleeWeaponWrapper);
		
		//현재 손에 든 무기 버리기
		EnhancedInputComponent->BindAction(DropCurrentWeaponAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::DropCurrentWeapon);

		//상호작용 하기
		EnhancedInputComponent->BindAction(IntercatAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::TryInteract);
	}
	
	if (bIsLocalPlayer)
	{
		
	}
}
