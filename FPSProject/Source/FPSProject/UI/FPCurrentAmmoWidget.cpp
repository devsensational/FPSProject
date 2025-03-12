// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCurrentAmmoWidget.h"

#include "Character/FPCharacterBase.h"
#include "Weapon/FPWeaponBase.h"

void UFPCurrentAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PlayerController) return;
	
	CurrentWeapon = Cast<AFPCharacterBase>(PlayerController->GetPawn())->GetCurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->OnAmmoChanged.AddDynamic(this, &UFPCurrentAmmoWidget::AmmoToText);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get current weapon"));
	}
}

void UFPCurrentAmmoWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (CurrentWeapon)
	{
		CurrentWeapon->OnAmmoChanged.RemoveAll(this);
	}
}

void UFPCurrentAmmoWidget::AmmoToText(int32 InCurrentAmmo, int32 InCurrentRemainingAmmo)
{
	SetTextValue(LexToString(InCurrentAmmo) + "/" + LexToString(InCurrentRemainingAmmo));
}
