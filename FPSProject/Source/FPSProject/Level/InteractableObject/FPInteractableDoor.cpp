// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/InteractableObject/FPInteractableDoor.h"

#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"

AFPInteractableDoor::AFPInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Mesh
	DoorPivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorPivot"));
	DoorPivot->SetupAttachment(RootComponent);
	VisualMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	VisualMesh->SetupAttachment(DoorPivot);
}

void AFPInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPInteractableDoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPInteractableDoor, bIsOpen);
}

void AFPInteractableDoor::Interact_Implementation(AFPCharacterBase* Interactor)
{
	Super::Interact_Implementation(Interactor);
	ToggleDoor();
}

void AFPInteractableDoor::ToggleDoor()
{
	if (HasAuthority())
	{
		bIsOpen = !bIsOpen;
		OnRep_DoorState();
	}
	else
	{
		ServerToggleDoor();
	}
}

void AFPInteractableDoor::OnRep_DoorState()
{
	PlayDoorAnimation(bIsOpen);
}

void AFPInteractableDoor::ServerToggleDoor_Implementation()
{
	ToggleDoor();
}
