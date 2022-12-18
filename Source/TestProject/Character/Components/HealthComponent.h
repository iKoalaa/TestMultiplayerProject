// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDieEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChanged);

USTRUCT(BlueprintType)
struct FHealthData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHealthData HealthData;

public:
	UPROPERTY(BlueprintAssignable)
	FDieEvent OnDie;
	UPROPERTY(BlueprintAssignable)
	FHealthChanged FOnHealthChanged;

public:
	UFUNCTION(NetMulticast, Reliable)
	void ApplyDamage(float Damage);
	
	UFUNCTION(BlueprintCallable)
	float GetHealth();
};
