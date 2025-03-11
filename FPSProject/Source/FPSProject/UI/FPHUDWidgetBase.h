// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPHUDWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPHUDWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	/* 스탯 업데이트 섹션 */
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHp(int32 CurrentHp);

	//FUNCTION(BlueprintCallable)
	//void UpdateAmmo(int32 CurrentAmmo);

protected:
	virtual void NativeConstruct() override;

	// 체력 UI 위젯 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFPTextWidgetBase> HpWidget;

	// 탄약 UI 위젯 
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UFPTextWidgetBase> AmmoWidget;
};
