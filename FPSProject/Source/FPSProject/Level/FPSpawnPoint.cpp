// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/FPSpawnPoint.h"

#include "FPSProject.h"
#include "Game/FPGameMode.h"
#include "Interface/FPSpawnManagerInterface.h"

AFPSpawnPoint::AFPSpawnPoint()
{
	
}

void AFPSpawnPoint::BeginPlay()
{
	if (HasAuthority()) Init(); // 서버 일때만 스폰포인트에 대한 레퍼런스 생성 및 설정
}

void AFPSpawnPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFPSpawnPoint::Init()
{
	Super::BeginPlay();
	LOG_NET(NetworkLog, Log, TEXT("Created Spawn Point"));

	// Todo: 스폰매니저 레퍼런스 지정
	if (UWorld* World = GetWorld())
	{
		if (AFPGameMode* GameMode = Cast<AFPGameMode>(World->GetAuthGameMode()))
		{
			// 성공적으로 캐스팅되었을 때의 로직
			SpawnManager = GameMode->GetSpawnManager();
		}
		else
		{
			// 캐스팅 실패 처리
			LOG_NET(NetworkLog, Warning, TEXT("Cast failed for AFPGameMode"));
		}
	}

	// 스폰 포인트 생성 시 스폰 매니저에 레퍼런스 등록
	if (SpawnManager)
	{
		SpawnManager->RegisterSpawnPoint(this);
	}
	else
	{
		// 캐스팅 실패 처리
		LOG_NET(NetworkLog, Warning, TEXT("SpawnManager Load failed"));
	}
}
