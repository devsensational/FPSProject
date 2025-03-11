// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPTextWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UFPTextWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Text UI")
	void SetTextValue(const FString& NewText);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayText;
};
