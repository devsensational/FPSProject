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

	/* PlayerController 레퍼런스 바인딩 섹션 */
protected:
	UPROPERTY()
	APlayerController* PlayerController;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/* Text 변경 섹션 */
public:
	UFUNCTION(BlueprintCallable, Category = "Text UI")
	virtual void SetTextValue(const FString& NewText);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayText;
};
