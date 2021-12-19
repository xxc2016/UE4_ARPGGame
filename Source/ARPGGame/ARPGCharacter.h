// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARPGCharacter.generated.h"

UCLASS()
class ARPGGAME_API AARPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	void CallRestartPlayer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartCrouch();

	UFUNCTION()
	void StopCrouch();

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	void StopAttack();

	// �Ƿ����ڹ���
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AAttack")
	bool IsAttacking; 

	// �����׶�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AAttack")
	int AttackStage = 0;

	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* AttackMontage;

	// ��̫����ί���¼�
	UFUNCTION()
	void AttackEndCanMove();
};