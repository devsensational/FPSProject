// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPDirectionIndicator.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPDirectionIndicator : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CrosshairTop;

public:
	void SetTargetLocation(FVector3d TargetLocation);
};
