// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/FPSpawnManagerBase.h"

#include "FPSpawnPoint.h"
#include "FPSProject.h"

// Sets default values
AFPSpawnManagerBase::AFPSpawnManagerBase()
{
	
}

// Called when the game starts or when spawned
void AFPSpawnManagerBase::BeginPlay()
{
	Super::BeginPlay();

	LOG_NET(NetworkLog, Log, TEXT("SpawnManager Init"));
	
}

void AFPSpawnManagerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AFPSpawnManagerBase::RegisterSpawnPoint(AFPSpawnPoint* SpawnPoint)
{
	SpawnPointList.Add(SpawnPoint);
	LOG_NET(NetworkLog, Log, TEXT("Register Spawn Point"));
	
}

void AFPSpawnManagerBase::GetSpawnPointLocation() const
{
}
