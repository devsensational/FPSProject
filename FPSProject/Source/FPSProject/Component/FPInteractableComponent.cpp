// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPInteractableComponent.h"

#include "Character/FPCharacterPlayer.h"
#include "Physics/FPCollision.h"


UFPInteractableComponent::UFPInteractableComponent()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionProfileName(CPROFILE_FPINTERACTTRIGGER);
	SetHiddenInGame(false);
	SetGenerateOverlapEvents(true);
	SetMobility(EComponentMobility::Movable);
}

void UFPInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* OwnerActor = GetOwner())
	{
		if (IFPHasMesh* HasMeshInterface = Cast<IFPHasMesh>(OwnerActor))
		{
			CachedMesh = HasMeshInterface->GetMainMesh();
			UE_LOG(LogTemp, Log, TEXT("Mesh cashed"));
		}
		else
		{
			// 폴백: 메시 직접 찾기
			CachedMesh = OwnerActor->FindComponentByClass<UMeshComponent>();
		}
	}
}

void UFPInteractableComponent::PostInitProperties()
{
	Super::PostInitProperties();
	
	SetBoxExtent(BoxSize);
}

void UFPInteractableComponent::Highlight(bool bEnable)
{
	if (!CachedMesh) return;

	CachedMesh->SetRenderCustomDepth(bEnable);

	// 필요 시 색상 지정 (Material에서 이 StencilValue를 기준으로 색상 분기)
	CachedMesh->SetCustomDepthStencilValue(bEnable ? 1 : 0);
}

void UFPInteractableComponent::Interact(AFPCharacterBase* Interactor) const
{
	if (AActor* OwnerActor = GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("%s 와 상호작용 실행"), *OwnerActor->GetName());

		// 캐릭터인지 확인
		if (OwnerActor->Implements<UFPInteractable>())
		{
			IFPInteractable::Execute_Interact(OwnerActor, Interactor);
		}
	}
}
