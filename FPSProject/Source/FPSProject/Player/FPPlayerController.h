// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFPPlayerController, Log, All);
/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFPPlayerController();
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
