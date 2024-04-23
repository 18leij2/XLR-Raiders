// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "XLRGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XLRTESTMULTIPLAYER_API AXLRGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class AXLRPlayerController* VictimController, AXLRPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);
};
