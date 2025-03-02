// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPGameMode.generated.h"

class AFPWeaponBase;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AFPGameMode();

	// 멀티플레이 섹션
private:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;

	// 무기 NetGUID 매핑
private:
	UPROPERTY()
	TMap<int32, AFPWeaponBase*> WeaponMap;

public:
	void RegisterWeaponID(AFPWeaponBase* InWeapon);
	void UnRegisterWeaponID(const AFPWeaponBase* InWeapon);
	void UnRegisterWeaponID(int32 InWeaponID);
	
	AFPWeaponBase*  GetWeaponByID(int32 InWeaponID);
};
