// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

void UHealthComponent::ApplyDamage_Implementation(float Damage)
{
	HealthData.Health -= Damage;
	FOnHealthChanged.Broadcast();
	if (HealthData.Health < 0001.f)
	{
		OnDie.Broadcast();
	}
}

float UHealthComponent::GetHealth()
{
	return HealthData.Health;
}
