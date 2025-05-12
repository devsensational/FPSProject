// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/FPHasMesh.h"
#include "Interface/FPInteractable.h"
#include "FPInteractableLevelObject.generated.h"

UCLASS(Blueprintable)
class FPSPROJECT_API AFPInteractableLevelObject : public AActor, public IFPInteractable, public IFPHasMesh
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

	// Interface
	virtual void Interact_Implementation(AFPCharacterBase* Interactor) override;
	FORCEINLINE virtual UMeshComponent* GetMainMesh() override { return VisualMesh; }
};
