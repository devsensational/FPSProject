// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHUDWidgetBase.h"

void UFPHUDWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (WBP_Crosshair)
	{
		UE_LOG(LogTemp, Log, TEXT("Crosshair Widget loaded"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Crosshair Widget NOT loaded"));
	}
	
}
