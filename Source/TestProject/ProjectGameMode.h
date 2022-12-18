// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectGameMode.generated.h"

class ABaseCharacter;

UCLASS()
class AProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectGameMode();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> SpawnPoint;

	UFUNCTION(Server, Reliable)
	void Respawn(ABaseCharacter* BaseCharacter, AController* PlayerController);
};



