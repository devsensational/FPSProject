// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPTextWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "FPCurrentHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPCurrentHpWidget : public UFPTextWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	
	/* 캐릭터 스탯 컴포넌트 섹션 */
protected:
	UPROPERTY()
	TObjectPtr<class AFPCharacterBase> PlayerCharacter;
	
	UPROPERTY()
	TObjectPtr<class UFPCharacterStatComponent> CharacterStatComponent;

	UFUNCTION()
	void HealthToText(float InHealth);
};
