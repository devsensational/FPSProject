// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FPGameMode.h"

#include "FPSProject.h"
#include "Interface/FPSpawnManagerInterface.h"
#include "Level/FPSpawnManagerBase.h"
#include "Weapon/FPWeaponBase.h"
#include "Character/FPCharacterBase.h"

AFPGameMode::AFPGameMode()
{
	
}

void AFPGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (SpawnManagerClass)
	{
		SpawnManager = NewObject<AFPSpawnManagerBase>(this, SpawnManagerClass);
		LOG_NET(NetworkLog, Log, TEXT("Created AFPSpawnManager"));
	}
	else
	{
		LOG_NET(NetworkLog, Log, TEXT("Create Failed AFPSpawnManagerB"));
	}
}

// 클라이언트가 서버에 접속을 요청했을 때 가장 먼저 호출되는 함수
// 플레이어가 실제로 서버에 접속하기 전, 접속을 허용할지 거부할지를 결정할 수 있습니다.
// 주로 IP 차단, 중복 로그인 확인, 밴 리스트 체크 등 사전 검증에 사용됩니다.
// ErrorMessage 변수에 값을 설정하면 클라이언트에게 연결 거부 이유를 전달할 수 있습니다.
// 서버: 호출됨, 클라이언트: 호출되지 않음
void AFPGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("============================================================"));
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	//ErrorMessage = TEXT("Server Is Full");

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));
}

// PreLogin()이 성공적으로 끝난 후, 플레이어가 서버에 최종적으로 연결을 시도할 때 호출됩니다.
// 서버에 플레이어 컨트롤러(PlayerController)를 생성하여 플레이어를 서버 세션에 추가합니다.
// 플레이어의 인증 정보나 옵션을 기반으로 추가적인 검증 또는 처리를 수행할 수 있습니다.
// 서버: 호출됨,클라이언트: 호출되지 않음
APlayerController* AFPGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

// 플레이어가 완전히 접속한 후(Login() 이후), 서버에 플레이어 컨트롤러가 성공적으로 등록된 직후에 호출됩니다.
// 플레이어 접속 이후의 추가 설정 작업(예: 웰컴 메시지 전송, 초기 위치 설정, UI 업데이트 등)을 수행합니다.
// 게임 내에 플레이어가 실제 존재하는 상태에서 호출되는 함수이므로 초기 게임 로직 처리에 유리합니다.
// 서버: 호출됨 클라이언트: 호출되지 않음 (단, PlayerController나 Pawn 등을 통해 클라이언트에게 간접적으로 이벤트를 전달할 수 있음)
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

// 서버가 최초로 게임을 시작할 때 호출됩니다. 이는 일반적으로 레벨이 로딩된 후, 첫 번째 프레임이 시작되기 직전에 호출됩니다.
// 게임 세션을 시작할 때 최초 게임 환경 구성, AI 초기화, 스폰 로직 실행 등을 수행합니다.
// 로그인과 관련되기보다는, 게임 세션의 시작 시점과 관련된 초기 설정이 이루어지는 시점으로 보는 것이 좋습니다.
// 서버: 호출됨, 클라이언트: 호출되지 않음 (클라이언트는 별도의 방식으로 게임이 시작되었음을 인지함)
void AFPGameMode::StartPlay()
{
	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	LOG_NET(NetworkLog, Log, TEXT("%s"), TEXT("End"));

}

void AFPGameMode::RegisterCharacterID(AFPCharacterBase* InCharacterReference)
{
	if (!InCharacterReference)
	{
		LOG_NET(NetworkLog, Warning, TEXT("Invalid Character Reference"));
		return;
	}

	int32 CharacterID = InCharacterReference->GetUniqueID();
	CharacterMap.Add(CharacterID, InCharacterReference);
	CharacterList.Add(InCharacterReference);
	
	LOG_NET(NetworkLog, Log, TEXT("%d: Register Character"), CharacterID);
}

void AFPGameMode::UnregisterCharacterReference(const AFPCharacterBase* InCharacterReference)
{
	if (!InCharacterReference)
	{
		LOG_NET(NetworkLog, Warning, TEXT("Invalid Character Reference"));
		return;
	}

	int32 CharacterID = InCharacterReference->GetUniqueID();
	if (!CharacterMap.Contains(CharacterID)) { return; }
	if (!CharacterList.Contains(InCharacterReference)) { return; }
	
	CharacterList.RemoveAll([InCharacterReference](const TObjectPtr<AFPCharacterBase>& Character)
	{
		return Character.Get() == InCharacterReference;
	});
	
	CharacterMap.Remove(CharacterID);
	
	LOG_NET(NetworkLog, Log, TEXT("%d: Unregister Character"), CharacterID);
}

void AFPGameMode::UnregisterCharacterReference(const int32 InCharacterID)
{
	if (!CharacterMap.Contains(InCharacterID))
	{
		LOG_NET(NetworkLog, Warning, TEXT("Invalid Character ID: %d"), InCharacterID);
		return;
	}
	
	AFPCharacterBase* CharacterReference = CharacterMap[InCharacterID];
	if (!CharacterList.Contains(CharacterReference)) { return; }
	CharacterList.RemoveAll([CharacterReference](const TObjectPtr<AFPCharacterBase>& Character)
	{
		return Character.Get() == CharacterReference;
	});
	
	CharacterMap.Remove(InCharacterID);
	
	LOG_NET(NetworkLog, Log, TEXT("%d: Unregister Character"), InCharacterID);
}

void AFPGameMode::RegisterWeaponID(AFPWeaponBase* InWeaponReference)
{
	if (InWeaponReference)
	{
		WeaponMap.Add(InWeaponReference->GetUniqueID(), InWeaponReference);
	}
	
	LOG_NET(NetworkLog, Log, TEXT("Register Weapon"));
}

void AFPGameMode::UnregisterWeaponID(const AFPWeaponBase* InWeaponReference)
{
	if (InWeaponReference)
	{
		int32 WeaponID = InWeaponReference->GetUniqueID();
		if (WeaponMap.Contains(WeaponID))
		{
			WeaponMap.Remove(WeaponID);
		}
	}
}

void AFPGameMode::UnregisterWeaponID(int32 InWeaponID)
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
