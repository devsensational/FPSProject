// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/InteractableObject/FPInteractableDoor.h"

#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"

AFPInteractableDoor::AFPInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
	
	// 초기 회전 값 설정 (닫힘)
	ClosedRotation = FRotator(0.0f, 0.0f, 0.0f);
	OpenRotation = FRotator(0.0f, 180.0f, 0.0f); // 열릴 때 90도 회전
}

void AFPInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (DoorCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("HandleDoorRotation"));
		DoorTimeline->AddInterpFloat(DoorCurve, TimelineCallback);
	}
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

void AFPInteractableDoor::HandleDoorRotation(float Value)
{
	FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, Value);
	VisualMesh->SetRelativeRotation(NewRotation);
}

void AFPInteractableDoor::OnRep_DoorState()
{
	PlayDoorAnimation(bIsOpen);
}

void AFPInteractableDoor::PlayDoorAnimation(bool bOpen)
{
	if (DoorTimeline->IsPlaying())
	{
		DoorTimeline->Stop();
	}

	if (bOpen)
	{
		DoorTimeline->PlayFromStart();
	}
	else
	{
		DoorTimeline->ReverseFromEnd();
	}
}

void AFPInteractableDoor::ServerToggleDoor_Implementation()
{
	ToggleDoor();
}