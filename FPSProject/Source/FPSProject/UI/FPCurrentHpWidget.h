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

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void SetEventManager(UFPGlobalEventManager* InEventManager) override;
	
	UFUNCTION()
	void HealthToText(float InHealth);
};
