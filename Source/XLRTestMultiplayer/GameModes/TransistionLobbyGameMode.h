// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TransistionLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XLRTESTMULTIPLAYER_API ATransistionLobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public: 
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
