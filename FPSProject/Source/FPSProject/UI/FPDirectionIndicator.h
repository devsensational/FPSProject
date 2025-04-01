// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPHUDWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "FPDirectionIndicator.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPDirectionIndicator : public UFPHUDWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* 타겟 설정 섹션 */
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ArrowImage;
	
private:
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

public:
	void SetTargetLocation(AActor* InTargetActor);
	
};
