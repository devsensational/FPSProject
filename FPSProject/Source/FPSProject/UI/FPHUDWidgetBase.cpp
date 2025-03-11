// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHUDWidgetBase.h"

#include "FPTextWidgetBase.h"

void UFPHUDWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 필요한 경우, 초기값을 설정 가능
    if (HpWidget) HpWidget->SetTextValue(TEXT("HP: 100"));
    //if (AmmoWidget) AmmoWidget->SetTextValue(TEXT("Ammo: 30"));
}

void UFPHUDWidgetBase::UpdateHp(int32 CurrentHp)
{   if (HpWidget)
    {
        HpWidget->SetTextValue(FString::Printf(TEXT("HP: %d"), CurrentHp));
    }
}

/*
void UFPHUDWidgetBase::UpdateAmmo(int32 CurrentAmmo)
{
    if (AmmoWidget)
    {
        AmmoWidget->SetTextValue(FString::Printf(TEXT("Ammo: %d"), CurrentAmmo));
    }
}
*/

