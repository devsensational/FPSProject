// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/FPInteractable.h"
#include "FPInteractTestActor.generated.h"

UCLASS()
class FPSPROJECT_API AFPInteractTestActor : public AActor, public IFPInteractable
{
	GENERATED_BODY()
	
public:	
	AFPInteractTestActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Mesh")
	UMeshComponent* Mesh;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UFPInteractableComponent> InteractableComponent;

public:
	virtual void Interact_Implementation(AFPCharacterBase* Interactor) override;
};
