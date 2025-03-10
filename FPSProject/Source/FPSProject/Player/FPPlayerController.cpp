// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FPPlayerController.h"
#include "FPSProject.h"
#include "Character/FPCharacterBase.h"
#include "CharacterStat/FPCharacterStatComponent.h"

DEFINE_LOG_CATEGORY(LogFPPlayerController);

AFPPlayerController::AFPPlayerController()
{
	
}

void AFPPlayerController::PostInitializeComponents()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}

void AFPPlayerController::PostNetInit()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ServerConnection)
		{
			LOG_NET(NetworkLog, Log, TEXT("Server Connection: %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}

void AFPPlayerController::BeginPlay()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

}

void AFPPlayerController::OnPossess(APawn* InPawn)
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);
	
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}
