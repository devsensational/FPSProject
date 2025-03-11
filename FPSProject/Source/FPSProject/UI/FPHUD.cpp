// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPHUD.h"

#include "FPSProject.h"
#include "Blueprint/UserWidget.h"
#include "Character/FPCharacterBase.h"
#include "CharacterStat/FPCharacterStatComponent.h"
#include "Player/FPPlayerController.h"

void AFPHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	PlayerCharacter = Cast<AFPCharacterBase>(GetOwningPawn());
	if (PlayerCharacter)
	{
		CharacterStatComponent = PlayerCharacter->GetCharacterStatComponent();
		CharacterStatComponent->OnHealthChanged.AddDynamic(this, &AFPHUD::UpdateHealth);
	}
}

void AFPHUD::UpdateHealth(float NewHealth)
{
	LOG_NET(NetworkLog, Log, TEXT("Health point evnet"));
}
