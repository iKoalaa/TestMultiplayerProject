// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGameInstance.generated.h"

UCLASS()

class TESTPROJECT_API UProjectGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

	virtual void Init() override;

public:
	
	UFUNCTION(BlueprintCallable)
	static FORCEINLINE UProjectGameInstance* GetManager(UWorld* World)
	{
		return Cast<UProjectGameInstance>(World->GetGameInstance());
	}
};
