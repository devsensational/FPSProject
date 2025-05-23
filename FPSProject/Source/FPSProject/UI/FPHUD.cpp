// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPHUD.h"

#include "FPHUDLayout.h"
#include "Blueprint/UserWidget.h"

void AFPHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = Cast<UFPHUDLayout>(CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass));
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}
