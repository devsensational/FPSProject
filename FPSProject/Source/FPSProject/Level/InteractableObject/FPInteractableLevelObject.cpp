// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/InteractableObject/FPInteractableLevelObject.h"

#include "Component/FPInteractableComponent.h"

AFPInteractableLevelObject::AFPInteractableLevelObject()
{
	// 3인칭용 메시
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// 상호작용 컴포넌트
	InteractableComponent = CreateDefaultSubobject<UFPInteractableComponent>(TEXT("InteractComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
	InteractableComponent->SetHiddenInGame(false); // 게임 중 보이도록 설정
}

void AFPInteractableLevelObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPInteractableLevelObject::Interact_Implementation(class AFPCharacterBase* Interactor)
{
	IFPInteractable::Interact_Implementation(Interactor);

	
}

