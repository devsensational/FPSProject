// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/FPGameSingleton.h"

UFPGameSingleton::UFPGameSingleton()
{
	// 데이터 테이블 로드
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/DT_WeaponStats.DT_WeaponStats'"));
	UE_LOG(LogTemp, Log, TEXT("Try load DataTable"));
	if (DataTableRef.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon datatable load"));
		if (WeaponDataTable)
		{
			// 데이터 테이블의 모든 행을 가져옴
			TArray<FName> RowNames = WeaponDataTable->GetRowNames();

			// 각 행을 순회하며 TMap에 저장
			for (const FName& RowName : RowNames)
			{
				// FindRow를 사용하여 안전하게 행 데이터 가져오기
				FWeaponStats* WeaponStats = WeaponDataTable->FindRow<FWeaponStats>(RowName, TEXT(""));

				if (WeaponStats)
				{
					WeaponStatsMap.Add(RowName, WeaponStats);
				}
			}

			// TMap에 저장된 데이터 출력 (예시)
			for (const auto& WeaponPair : WeaponStatsMap)
			{
				FName WeaponName = WeaponPair.Key;
				FWeaponStats* WeaponStats = WeaponPair.Value;

				if (WeaponStats)
				{
					UE_LOG(LogTemp, Log, TEXT("Weapon Name: %s"), 
						*WeaponName.ToString(), WeaponStats->Damage);
				}
			}
		}
	}
}

UFPGameSingleton& UFPGameSingleton::Get()
{
	UFPGameSingleton* Singleton = CastChecked<UFPGameSingleton>(GEngine->GameSingleton);
	if (Singleton) return *Singleton;
	
	UE_LOG(LogTemp, Error, TEXT("UFPGameSingleton::Get()"));
	return *NewObject<UFPGameSingleton>();
}
