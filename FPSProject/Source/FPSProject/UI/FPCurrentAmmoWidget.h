// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FPTextWidgetBase.h"
#include "FPCurrentAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPCurrentAmmoWidget : public UFPTextWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void SetEventManager(UFPGlobalEventManager* InEventManager) override;
	/* Weapon 레퍼런스 섹션 */
protected:
	UFUNCTION()
	void AmmoToText(int32 InCurrentAmmo, int32 InCurrentRemainingAmmo);
};
