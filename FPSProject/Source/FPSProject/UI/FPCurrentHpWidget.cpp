// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCurrentHpWidget.h"

#include "Character/FPCharacterBase.h"
#include "CharacterStat/FPCharacterStatComponent.h"

void UFPCurrentHpWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFPCurrentHpWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (EventManager)
	{
		EventManager->OnHealthChanged.RemoveAll(this);
	}
}

void UFPCurrentHpWidget::SetEventManager(UFPGlobalEventManager* InEventManager)
{
	Super::SetEventManager(InEventManager);
	EventManager->OnHealthChanged.AddDynamic(this, &UFPCurrentHpWidget::HealthToText);
}

void UFPCurrentHpWidget::HealthToText(float InHealth)
{
	SetTextValue(LexToString(static_cast<int32>(InHealth)));
}
