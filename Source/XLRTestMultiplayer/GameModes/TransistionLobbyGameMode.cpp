// Fill out your copyright notice in the Description page of Project Settings.


#include "TransistionLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ATransistionLobbyGameMode::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == LobbySize)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/MatchLevel?listen"));
		}
	}
}