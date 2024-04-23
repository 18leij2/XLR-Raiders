// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XLRTestMultiplayer/XLRComponents/CombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "XLRTestMultiplayer/XLRTestMultiplayer.h"
#include "XLRTestMultiplayer/PlayerController/XLRPlayerController.h"
#include "XLRTestMultiplayer/GameModes/XLRGameMode.h"
#include "TimerManager.h"
// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUDHealth();

	if (HasAuthority()) 
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);
	}

}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterCharacter, OverlappingWeapon);

	DOREPLIFETIME(ABlasterCharacter, Health);
}


// Called to bind functionality to input
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABlasterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABlasterCharacter::FireButtonReleased);


	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
}


void ABlasterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABlasterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABlasterCharacter::FireButtonPressed()
{
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void ABlasterCharacter::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}


// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlasterCharacter::EquipButtonPressed()
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void ABlasterCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	//Play Hit Reaction
	
}


void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser)
{

	FVector2D WalkSpeedRange(0.f, Cast<ABlasterCharacter>(DamageCauser->GetOwner())->GetCharacterMovement()->MaxWalkSpeed);
	FVector2D VelocityMultiplierRange(1.f, 2.f);
	float ModifiedVelocity = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, DamageCauser->GetOwner()->GetVelocity().Size());
	float ModifiedDamage = Damage * ModifiedVelocity;
	Health = FMath::Clamp(Health - ModifiedDamage, 0.f, MaxHealth);
	UpdateHUDHealth();
	//Play Hit Reaction
	
	if (Health == 0.f)
	{
		AXLRGameMode* XLRGameMode = GetWorld()->GetAuthGameMode<AXLRGameMode>();
		if (XLRGameMode)
		{
			XLRPlayerController = XLRPlayerController == nullptr ? Cast<AXLRPlayerController>(Controller) : XLRPlayerController;
			AXLRPlayerController* AttackerController = Cast<AXLRPlayerController>(InstigatorController);
			XLRGameMode->PlayerEliminated(this, XLRPlayerController, AttackerController);
		}
	}

}

void ABlasterCharacter::UpdateHUDHealth()
{
	XLRPlayerController = XLRPlayerController == nullptr ? Cast<AXLRPlayerController>(Controller) : XLRPlayerController;
	if (XLRPlayerController)
	{
		XLRPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABlasterCharacter::Elim()
{
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ABlasterCharacter::ElimTimerFinished,
		ElimDelay
	);
}

void ABlasterCharacter::MulticastElim_Implementation()
{
	bElim = true;
	PlayDeathMontage();
}

void ABlasterCharacter::ElimTimerFinished()
{
	AXLRGameMode* XLRGameMode = GetWorld()->GetAuthGameMode<AXLRGameMode>();
	if (XLRGameMode)
	{
		XLRGameMode->RequestRespawn(this, Controller);
		UpdateHUDHealth();
	}
}

void ABlasterCharacter::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
}

