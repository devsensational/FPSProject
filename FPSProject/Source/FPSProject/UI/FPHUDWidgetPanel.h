// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPHUDWidgetPanel.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPHUDWidgetPanel : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	void EventBindInitialize();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFPCrosshair> WBP_Crosshair;
	
	// 체력 UI 위젯 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFPTextWidgetBase> WBP_HpWidget;

	// 장탄 수 UI 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UFPTextWidgetBase> WBP_CurrentAmmo;
	
};
