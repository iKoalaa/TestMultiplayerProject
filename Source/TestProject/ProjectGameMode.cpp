// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGameMode.h"

#include "Character/Actors/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

class ABaseCharacter;

AProjectGameMode::AProjectGameMode() : Super()
{
}

void AProjectGameMode::Respawn_Implementation(ABaseCharacter* BaseCharacter, AController* Controller)
{
	if (BaseCharacter)
	{
		BaseCharacter->Reset();
		BaseCharacter->Destroy();
	}

	if (Controller)
	{
		RestartPlayerAtPlayerStart(Controller, SpawnPoint.Get());
	}
}
