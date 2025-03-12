// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPHUD.h"

#include "FPHUDWidgetBase.h"
#include "Blueprint/UserWidget.h"

void AFPHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = Cast<UFPHUDWidgetBase>(CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass));
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}
