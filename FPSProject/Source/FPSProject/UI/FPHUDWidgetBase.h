// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/FPGlobalEventManager.h"
#include "FPHUDWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPHUDWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void SetEventManager(UFPGlobalEventManager* InEventManager) { EventManager = InEventManager; }

protected:
	UPROPERTY()
	TObjectPtr<UFPGlobalEventManager> EventManager;
	
};
