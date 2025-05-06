// Fill out your copyright notice in the Description page of Project Settings.


#include "FPInteractTestActor.h"

#include "FPSProject.h"
#include "Component/FPInteractableComponent.h"

// Sets default values
AFPInteractTestActor::AFPInteractTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableComponent = CreateDefaultSubobject<UFPInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
}

void AFPInteractTestActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFPInteractTestActor::Interact_Implementation(AFPCharacterBase* Interactor)
{
	IFPInteractable::Interact_Implementation(Interactor);
	LOG_NET(NetworkLog, Log, TEXT("Interaction"));
}

