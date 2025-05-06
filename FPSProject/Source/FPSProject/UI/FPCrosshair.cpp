// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCrosshair.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UFPCrosshair::NativeConstruct()
{
	Super::NativeConstruct();

	CrosshairImages.Add(CrosshairTop);
	CrosshairImages.Add(CrosshairBottom);
	CrosshairImages.Add(CrosshairLeft);
	CrosshairImages.Add(CrosshairRight);

	UpdateCrosshairSpread(2.0f);
	SetCrosshairSize(10.0f, 3.0f);
	SetCrosshairColor(255, 0 ,0, 1.0f);
}

void UFPCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//float Spread = *CharacterAccuracy * SpreadMultiplier;
	float Spread = 1.0f;
	if (CrosshairTop)    CrosshairTop->SetRenderTranslation(FVector2D(0, -Spread));
	if (CrosshairBottom) CrosshairBottom->SetRenderTranslation(FVector2D(0, Spread));
	if (CrosshairLeft)   CrosshairLeft->SetRenderTranslation(FVector2D(-Spread, 0));
	if (CrosshairRight)  CrosshairRight->SetRenderTranslation(FVector2D(Spread, 0));
}

void UFPCrosshair::SetCrosshairSize(float Width, float Height)
{
	const FVector2D NewSize(Width, Height);
	for (const auto Element : CrosshairImages)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Element->Slot);
		CanvasSlot->SetSize(NewSize);
	}
}

void UFPCrosshair::SetCrosshairColor(float R, float G, float B, float A)
{
	FLinearColor NewColor(R, G, B, A);
	for (const auto Element : CrosshairImages)
	{
		Element->SetColorAndOpacity(NewColor);
	}
}
void UFPCrosshair::UpdateCrosshairSpread(float Accuracy)
{
	const float Spread = Accuracy * SpreadMultiplier;

	if (CrosshairTop)    CrosshairTop->SetRenderTranslation(FVector2D(0, -Spread));
	if (CrosshairBottom) CrosshairBottom->SetRenderTranslation(FVector2D(0, Spread));
	if (CrosshairLeft)   CrosshairLeft->SetRenderTranslation(FVector2D(-Spread, 0));
	if (CrosshairRight)  CrosshairRight->SetRenderTranslation(FVector2D(Spread, 0));
}
