// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPHUDWidgetBase.h"
#include "FPCrosshair.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPCrosshair : public UFPHUDWidgetBase
{
	GENERATED_BODY()
	
public:
	// 조준선 간격 설정 (Accuracy 값을 받아서 거리 계산)
	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void UpdateCrosshairSpread(float Accuracy);
	
	void SetCrosshairSize(float Width, float Height);
	void SetCrosshairColor(float R, float G, float B, float A);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CrosshairTop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>  CrosshairBottom;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>  CrosshairLeft;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>  CrosshairRight;


private:
	UPROPERTY()
	TArray<TObjectPtr<UImage>> CrosshairImages;
	
	// 내부 거리 계산용
	float SpreadMultiplier = 15.0f; // Accuracy와 곱해서 거리 계산
};
