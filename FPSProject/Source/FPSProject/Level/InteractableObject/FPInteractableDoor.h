// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/InteractableObject/FPInteractableLevelObject.h"
#include "FPInteractableDoor.generated.h"

class UTimelineComponent;
/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPInteractableDoor : public AFPInteractableLevelObject
{
	GENERATED_BODY()

public:
	AFPInteractableDoor();
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* 문 상호작용 (토글) */
public:
	virtual void Interact_Implementation(AFPCharacterBase* Interactor) override;
	
	// 문 열기 / 닫기 (서버에서 호출)// 문 열림 상태 (Replicated)
	UPROPERTY(ReplicatedUsing = OnRep_DoorState)
	bool bIsOpen;
	
	UFUNCTION(BlueprintCallable)
	void ToggleDoor();

	UPROPERTY()
	TObjectPtr<UTimelineComponent> DoorTimeline;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DoorCurve;
	
	UFUNCTION()
	void HandleDoorRotation(float Value);

	UFUNCTION()
	void OnRep_DoorState();

	// 서버에서 상태를 전환
	UFUNCTION(Server, Reliable)
	void ServerToggleDoor();

	void PlayDoorAnimation(bool bOpen);
	
	FRotator ClosedRotation;
	FRotator OpenRotation;
};
