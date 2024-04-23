// Fill out your copyright notice in the Description page of Project Settings.


#include "XLRGameMode.h"
#include "XLRTestMultiplayer/Character/BlasterCharacter.h"
#include "XLRTestMultiplayer/PlayerController/XLRPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void AXLRGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class AXLRPlayerController* VictimController, AXLRPlayerController* AttackerController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AXLRGameMode::RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController) {
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> Spawns;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), Spawns);
		int32 Selection = FMath::RandRange(0, Spawns.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, Spawns[Selection]);
	}
}