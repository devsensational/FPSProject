// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPCharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
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

//입력 관련 메소드
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


void AFPCharacterPlayer::SetupPlayerInputComponent(UInputComponent*		PlayerInputComponent)
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
		EnhancedInputComponent->BindAction(EquipPrimaryAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::EquipPrimaryWeapon);

		//보조무기 장착
		EnhancedInputComponent->BindAction(EquipSecondaryAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::EquipSecondaryWeapon);

		//근접무기 장착
		EnhancedInputComponent->BindAction(EquipMeleeAction, ETriggerEvent::Triggered, this, &AFPCharacterPlayer::EquipMeleeWeapon);
	}
	
	if (bIsLocalPlayer)
	{
		
	}
}
