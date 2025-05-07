// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/FPInteractable.h"
#include "FPInteractableLevelObject.generated.h"

UCLASS()
class FPSPROJECT_API AFPInteractableLevelObject : public AActor, public IFPInteractable
{
	GENERATED_BODY()
	
public:	
	AFPInteractableLevelObject();

protected:
	virtual void BeginPlay() override;
	
	/* Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> VisualMesh;

	/* 상호작용 섹션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UFPInteractableComponent> InteractableComponent;
	
	virtual void Interact_Implementation(AFPCharacterBase* Interactor) override;
};
