// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


#define TRACE_LENGTH 80000
class AWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class XLRTESTMULTIPLAYER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	friend class ABlasterCharacter;

	void EquipWeapon(class AWeapon* WeaponToEQuip);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FireButtonPressed(bool bPressed);

	void Fire();

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	UFUNCTION(Server, Reliable)
		void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void SetHUDCrosshairs(float DeltaTime);

private:
	class ABlasterCharacter* Character;
	class AXLRPlayerController* Controller;
	class AXLRHUD* HUD;

	UPROPERTY(Replicated)
	AWeapon* EquippedWeapon;

	bool bFireButtonPressed;


	//HUD

	float CrosshairVelocityFactor;

	//Automatic Fire

	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	FVector HitTarget;

public:
	// Called every frame


};
