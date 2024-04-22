// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class XLRTESTMULTIPLAYER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Elim();
	void PlayDeathMontage();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void FireButtonPressed();
	void FireButtonReleased();

	void EquipButtonPressed();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
		class UCombatComponent* Combat;

	UPROPERTY(Replicated)
		class AWeapon* OverlappingWeapon;

	UFUNCTION(Server, Reliable)
		void ServerEquipButtonPressed();

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* DeathMontage;

	//Player Health
	
	UPROPERTY(EditAnywhere, Category = PlayerStats)
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = PlayerStats)
		float Health = 100.f;

	UFUNCTION()
		void OnRep_Health();

	bool bElim = false;


	class AXLRPlayerController* XLRPlayerController;

	

public:
	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	FORCEINLINE bool IsElimmed() const { return bElim;  }

};
