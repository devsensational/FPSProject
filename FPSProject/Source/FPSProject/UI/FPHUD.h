// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPHUD.generated.h"

class UFPHUDLayout;
/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROJECT_API AFPHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// UI 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// 현재 활성화된 UI 위젯
	UPROPERTY()
	UFPHUDLayout* HUDWidget;

	/* 캐릭터 컨트롤러 섹션 */
	UPROPERTY()
	TObjectPtr<class AFPPlayerController> PlayerController;
};
