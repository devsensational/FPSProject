// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FPGameState.h"
#include "FPSProject.h"

void AFPGameState::HandleBeginPlay()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}

void AFPGameState::OnRep_ReplicatedHasBegunPlay()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}