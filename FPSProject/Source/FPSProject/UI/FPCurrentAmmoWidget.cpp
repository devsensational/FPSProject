// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCurrentAmmoWidget.h"
#include "Game/FPGlobalEventManager.h"

void UFPCurrentAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsRunningGame())
	{
		EventManager = GetGameInstance()->GetSubsystem<UFPGlobalEventManager>();
	}
	
	if (EventManager)
	{
		EventManager->OnAmmoChanged.AddDynamic(this, &UFPCurrentAmmoWidget::AmmoToText);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed get Event manager"));
	}
}

void UFPCurrentAmmoWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (EventManager)
	{
		EventManager->OnAmmoChanged.RemoveAll(this);
	}
}

void UFPCurrentAmmoWidget::AmmoToText(int32 InCurrentAmmo, int32 InCurrentRemainingAmmo)
{
	
	SetTextValue(LexToString(InCurrentAmmo) + "/" + LexToString(InCurrentRemainingAmmo));
}
