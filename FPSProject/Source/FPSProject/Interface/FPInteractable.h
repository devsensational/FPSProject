// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSPROJECT_API IFPInteractable
{
	GENERATED_BODY()

public:
	// 이 함수는 반드시 UFUNCTION으로 선언해야 Execute_Interact 가 생성됨
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(class AFPCharacterBase* Interactor);
};
