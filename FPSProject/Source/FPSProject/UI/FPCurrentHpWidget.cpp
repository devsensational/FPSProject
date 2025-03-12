// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCurrentHpWidget.h"

#include "Character/FPCharacterBase.h"
#include "CharacterStat/FPCharacterStatComponent.h"

void UFPCurrentHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PlayerController) return;
	

	PlayerCharacter = Cast<AFPCharacterBase>(PlayerController->GetPawn());
	if (PlayerCharacter)
	{
		CharacterStatComponent = PlayerCharacter->GetCharacterStatComponent();
		CharacterStatComponent->OnHealthChanged.AddDynamic(this, &UFPCurrentHpWidget::HealthToText);
	}
}

void UFPCurrentHpWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (CharacterStatComponent)
	{
		CharacterStatComponent->OnHealthChanged.RemoveAll(this);
	}
}

void UFPCurrentHpWidget::HealthToText(float InHealth)
{
	SetTextValue(LexToString(static_cast<int32>(InHealth)));
}
