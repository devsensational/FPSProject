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

protected:

	// 체력 UI 위젯 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFPTextWidgetBase> HpWidget;
};
