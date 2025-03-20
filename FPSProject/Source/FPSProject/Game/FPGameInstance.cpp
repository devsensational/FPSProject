// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FPGameInstance.h"
#include "GameData/FPWeaponStats.h"
#include "Engine/DataTable.h"

bool UFPGameInstance::GetWeaponStatsByName(FName WeaponName, FPWeaponStats& OutWeaponStats) const
{
	if (!WeaponDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable is not set in GameInstance!"));
		return false;
	}
	
	FPWeaponStats* FoundWeaponStats = WeaponDataTable->FindRow<FPWeaponStats>(WeaponName, TEXT("Weapon Lookup"));

	if (FoundWeaponStats)
	{
		OutWeaponStats = *FoundWeaponStats;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon with name '%s' not found in DataTable!"), *WeaponName.ToString());
	return false;
}
