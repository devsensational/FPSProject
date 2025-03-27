// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCurrentAmmoWidget.h"

#include "FPSProject.h"
#include "Game/FPGlobalEventManager.h"

void UFPCurrentAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UFPCurrentAmmoWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (EventManager)
	{
		EventManager->OnAmmoChanged.RemoveAll(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Cant find EventManager"));
	}
}

void UFPCurrentAmmoWidget::SetEventManager(UFPGlobalEventManager* InEventManager)
{
	Super::SetEventManager(InEventManager);
	if (EventManager)
	{
		EventManager->OnAmmoChanged.AddDynamic(this, &UFPCurrentAmmoWidget::AmmoToText);
	}
}

void UFPCurrentAmmoWidget::AmmoToText(int32 InCurrentAmmo, int32 InCurrentRemainingAmmo)
{
	
	SetTextValue(LexToString(InCurrentAmmo) + "/" + LexToString(InCurrentRemainingAmmo));
}
