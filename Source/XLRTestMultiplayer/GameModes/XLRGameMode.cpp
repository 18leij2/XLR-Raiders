// Fill out your copyright notice in the Description page of Project Settings.


#include "XLRGameMode.h"
#include "XLRTestMultiplayer/Character/BlasterCharacter.h"
#include "XLRTestMultiplayer/PlayerController/XLRPlayerController.h"

void AXLRGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class AXLRPlayerController* VictimController, AXLRPlayerController* AttackerController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}