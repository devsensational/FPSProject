// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/FPSpawnManagerInterface.h"
#include "FPGameMode.generated.h"

class AFPWeaponBase;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPGameMode();
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/* 멀티플레이 섹션 */
private:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;

	/* 무기 NetGUID 매핑 섹션 */
private:
	UPROPERTY()
	TMap<int32, AFPWeaponBase*> WeaponMap;

public:
	void RegisterWeaponID(AFPWeaponBase* InWeapon);
	void UnRegisterWeaponID(const AFPWeaponBase* InWeapon);
	void UnRegisterWeaponID(int32 InWeaponID);
	
	AFPWeaponBase*  GetWeaponByID(int32 InWeaponID);

	/* 스폰 매니저 섹션 */
public:
	// 블루프린트에서 직접 클래스 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSystem")
	TSubclassOf<class AFPSpawnManagerBase> SpawnManagerClass;

private:
	UPROPERTY()
	TScriptInterface<IFPSpawnManagerInterface> SpawnManager;

public:
	TScriptInterface<IFPSpawnManagerInterface> GetSpawnManager() const { return SpawnManager; };
};
