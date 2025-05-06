// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Interface/FPInteractable.h"
#include "FPInteractableComponent.generated.h"

/**
 * 
 */

UCLASS()
class FPSPROJECT_API UFPInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	FVector BoxSize = FVector(100.f, 100.f, 100.f);
	
	UFPInteractableComponent();
	
	// 외곽선 표시
	void Highlight(bool bEnable);
	void Interact(AFPCharacterBase* Interactor) const;
	
private:
	UPROPERTY()
	TObjectPtr<UMeshComponent> CachedMesh; // 대상 Mesh 보관용

	UPROPERTY()
	TObjectPtr<USceneComponent> RootComp; // 루트 컴포넌트 가져오기
	
};
