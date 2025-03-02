// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FPGameMode.h"

#include "FPSProject.h"
#include "Weapon/FPWeaponBase.h"

AFPGameMode::AFPGameMode()
{
	
}

void AFPGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("============================================================"));
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	//ErrorMessage = TEXT("Server Is Full");

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* AFPGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

void AFPGameMode::PostLogin(APlayerController* NewPlayer)
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("No Client Connection"));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				LOG_NET(NetworkLog, Log, TEXT("Client Connections: %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}

void AFPGameMode::StartPlay()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));

}

void AFPGameMode::RegisterWeaponID(AFPWeaponBase* InWeapon)
{
	if (InWeapon)
	{
		WeaponMap.Add(InWeapon->GetUniqueID(), InWeapon);
	}
}

void AFPGameMode::UnRegisterWeaponID(const AFPWeaponBase* InWeapon)
{
	if (InWeapon)
	{
		int32 WeaponID = InWeapon->GetUniqueID();
		if (WeaponMap.Contains(WeaponID))
		{
			WeaponMap.Remove(WeaponID);
		}
	}
}

void AFPGameMode::UnRegisterWeaponID(int32 InWeaponID)
{
	if (WeaponMap.Contains(InWeaponID))
	{
		WeaponMap.Remove(InWeaponID);
	}
}

AFPWeaponBase* AFPGameMode::GetWeaponByID(int32 InWeaponID)
{
	if (WeaponMap.Contains(InWeaponID))
	{
		LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Find Weapon"));
		return WeaponMap[InWeaponID];
	}
	return nullptr;
}
