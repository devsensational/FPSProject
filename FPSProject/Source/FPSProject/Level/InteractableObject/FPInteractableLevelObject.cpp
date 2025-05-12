// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/InteractableObject/FPInteractableLevelObject.h"

#include "Component/FPInteractableComponent.h"

AFPInteractableLevelObject::AFPInteractableLevelObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	// 메시
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);
	
	// 상호작용 컴포넌트
	InteractableComponent = CreateDefaultSubobject<UFPInteractableComponent>(TEXT("InteractComponent"));
	InteractableComponent->SetupAttachment(VisualMesh);
	InteractableComponent->SetHiddenInGame(false); // 게임 중 보이도록 설정
	InteractableComponent->SetGenerateOverlapEvents(true);
	
}

void AFPInteractableLevelObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPInteractableLevelObject::Interact_Implementation(class AFPCharacterBase* Interactor)
{
	IFPInteractable::Interact_Implementation(Interactor);

	
}

