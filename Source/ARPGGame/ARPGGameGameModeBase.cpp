// Copyright Epic Games, Inc. All Rights Reserved.


#include "ARPGGameGameModeBase.h"
#include "ARPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AARPGGameGameModeBase::AARPGGameGameModeBase()
{
	//��Ĭ��Pawn������Ϊ�ѻ�����ͼ�Ľ�ɫ
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Blueprints/BP_ARPGCharacter.BP_ARPGCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}


void AARPGGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("GameMode Start"));

	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this,&AARPGGameGameModeBase::PlayerDied);
	}
}

void AARPGGameGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AARPGGameGameModeBase::PlayerDied(ACharacter* Character)
{
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}
