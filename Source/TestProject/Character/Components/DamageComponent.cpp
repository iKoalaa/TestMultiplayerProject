// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageComponent.h"

#include "HealthComponent.h"

void UDamageComponent::ApplyDamage(float Damage)
{
	if(const TObjectPtr<UHealthComponent> HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyDamage(Damage);
	}
}
