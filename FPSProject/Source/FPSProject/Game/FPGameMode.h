// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/FPSpawnManagerInterface.h"

class AFPCharacterBase;
class AFPWeaponBase;
class AFPSpawnManagerBase;

#include "FPGameMode.generated.h"

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

	/* 캐릭터 NetGUID 맵핑 섹션 */
private:
	UPROPERTY()
	TMap<int32, TObjectPtr<AFPCharacterBase>> CharacterMap;

	UPROPERTY()
	TArray<TObjectPtr<AFPCharacterBase>> CharacterList;

public:
	void RegisterCharacterID(AFPCharacterBase* InCharacterReference);
	void UnregisterCharacterReference(const AFPCharacterBase* InCharacterReference);
	void UnregisterCharacterReference(const int32 InCharacterID);
	
	/* 무기 NetGUID 맵핑 섹션 */
private:
	UPROPERTY()
	TMap<int32, TObjectPtr<AFPWeaponBase>> WeaponMap;

public:
	void RegisterWeaponID(AFPWeaponBase* InWeaponReference);
	void UnregisterWeaponID(const AFPWeaponBase* InWeaponReference);
	void UnregisterWeaponID(int32 InWeaponID);
	
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
