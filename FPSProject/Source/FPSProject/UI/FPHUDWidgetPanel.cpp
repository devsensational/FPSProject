// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHUDWidgetPanel.h"

#include "FPCrosshair.h"
#include "FPDirectionIndicator.h"
#include "FPTextWidgetBase.h"

void UFPHUDWidgetPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFPHUDWidgetPanel::EventBindInitialize()
{
	UFPGlobalEventManager* EventManager = GetGameInstance()->GetSubsystem<UFPGlobalEventManager>();
	if (WBP_HpWidget)
	{
		WBP_HpWidget->SetEventManager(EventManager);
	}

	if (WBP_CurrentAmmo)
	{
		WBP_CurrentAmmo->SetEventManager(EventManager);
	}
	
	if (WBP_Crosshair)
	{
		WBP_Crosshair->SetEventManager(EventManager);
	}

	if (WBP_DirectionIndicator)
	{
		WBP_DirectionIndicator->SetEventManager(EventManager);
	}
	
	if (IsRunningGame())
	{
		//EventManager = GetGameInstance()->GetSubsystem<UFPGlobalEventManager>();
	}
	
}
