// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARPGGameGameModeBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AARPGCharacter::AARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARPGCharacter::Destroyed()
{
	Super::Destroyed();

	if (UWorld* World = GetWorld())
	{
		if (AARPGGameGameModeBase* GameMode = Cast<AARPGGameGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}

void AARPGCharacter::CallRestartPlayer()
{
	AController* ControllerRef = GetController();
	Destroy();
	if (UWorld* World = GetWorld())
	{
		if (AARPGGameGameModeBase* GameMode = Cast<AARPGGameGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(ControllerRef);
		}
	}
}

// Called every frame
void AARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AARPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARPGCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, &AARPGCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released,this, &AARPGCharacter::StopJump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AARPGCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AARPGCharacter::StopCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AARPGCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AARPGCharacter::StopSprint);
	//PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AARPGCharacter::CallRestartPlayer);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AARPGCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AARPGCharacter::StopAttack);
}

void AARPGCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	
}

void AARPGCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AARPGCharacter::StartJump()
{
	bPressedJump = true;
}

void AARPGCharacter::StopJump()
{
	bPressedJump = false;
}

void AARPGCharacter::StartCrouch()
{
	if (!GetCharacterMovement()->IsFalling())
	Crouch();
}

void AARPGCharacter::StopCrouch()
{
	if(!GetCharacterMovement()->IsFalling())
	UnCrouch();
}

void AARPGCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AARPGCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AARPGCharacter::StartAttack()
{
	if (!GetCharacterMovement()->IsWalking() || bIsCrouched)
	{
		return;
	}
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AttackMontage != nullptr && AnimInstance != nullptr)
	{
		if (!AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			IsAttacking = true;
			GetCharacterMovement()->SetMovementMode(MOVE_None);			
			float dt = AnimInstance->Montage_Play(AttackMontage);
			FOnMontageBlendingOutStarted MontageBlendDelegate;
			MontageBlendDelegate.BindUFunction(this, TEXT("AttackEndCanMove"));
			AnimInstance->Montage_SetBlendingOutDelegate(MontageBlendDelegate);
		}
	}
	
}
void AARPGCharacter::AttackEndCanMove()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
void AARPGCharacter::StopAttack()
{
	IsAttacking = false;
	
	//GetMovementComponent()->Activate();
}




