// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPTextWidgetBase.h"

#include "Components/TextBlock.h"

void UFPTextWidgetBase::SetTextValue(const FString& NewText)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(NewText));
	}
}
