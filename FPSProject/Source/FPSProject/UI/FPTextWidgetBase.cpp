// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPTextWidgetBase.h"

#include "FPHUD.h"
#include "FPSProject.h"
#include "Components/TextBlock.h"

void UFPTextWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	// Todo: 하위 클래스에서 이벤트 구독 시 해당 함수 오버라이드 하여 구현
	// 소유한 플레이어 컨트롤러 가져오기
	PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
	}
}

void UFPTextWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
	//Todo: 하위 클래스에서 이벤트를 지정했다면 이 함수를 오버라이드하여 바인드를 해제할 것
}

void UFPTextWidgetBase::SetTextValue(const FString& NewText)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(NewText));
	}
}
