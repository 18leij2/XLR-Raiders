// Fill out your copyright notice in the Description page of Project Settings.


#include "XLRPlayerController.h"
#include "XLRTestMultiplayer/HUD/XLRHUD.h"
#include "XLRTestMultiplayer/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"

void AXLRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	XLRHUD = Cast<AXLRHUD>(GetHUD());
}

void AXLRPlayerController::SetHUDHealth(float Health, float MaxHealth) 
{
	XLRHUD = XLRHUD == nullptr ? Cast<AXLRHUD>(GetHUD()) : XLRHUD;
	if (XLRHUD && XLRHUD->CharacterOverlay && XLRHUD->CharacterOverlay->HealthBar) 
	{
		const float HealthPercent = Health / MaxHealth;
		XLRHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
	}
}