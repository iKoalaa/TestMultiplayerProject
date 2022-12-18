// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponHolderComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "TestProject/Character/Actors/BaseCharacter.h"
#include "TestProject/Weapon/Actors/BaseWeapon.h"

UWeaponHolderComponent::UWeaponHolderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponHolderComponent::AddWeapon_Implementation(ABaseWeapon* NewWeapon)
{
	if(!OwnerCharacter)
	{
		return;
	}
	
	Weapons.Add(NewWeapon);
	NewWeapon->SetOwner(Cast<AActor>(OwnerCharacter));
	NewWeapon->SetInstigator(Cast<APawn>(OwnerCharacter));
	if(ActiveWeaponIndex == -1)
	{
		ActiveWeaponIndex = 0;
		PickWeapon();
	}
	else
	{
		NewWeapon->SK_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetActiveWeapon().SK_Weapon->SetSimulatePhysics(false);
		NewWeapon->SetActorHiddenInGame(true);
		NewWeapon->AttachToActor(OwnerCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewWeapon->SetActorTransform(OwnerCharacter->GetBackWeaponTransform());
	}
}

void UWeaponHolderComponent::PickWeapon_Implementation()
{
	GetActiveWeapon().SetActorHiddenInGame(false);
	GetActiveWeapon().SK_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetActiveWeapon().SK_Weapon->SetSimulatePhysics(false);
	GetActiveWeapon().AttachToActor(OwnerCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetActiveWeapon().SetActorRelativeTransform(OwnerCharacter->GetWeaponToFireTransform());
}

void UWeaponHolderComponent::HideActiveWeapon_Implementation()
{
	GetActiveWeapon().SetActorHiddenInGame(true);
	GetActiveWeapon().SK_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetActiveWeapon().SK_Weapon->SetSimulatePhysics(false);
	GetActiveWeapon().DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	GetActiveWeapon().AttachToActor(OwnerCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetActiveWeapon().SetActorTransform(OwnerCharacter->GetBackWeaponTransform());
}

void UWeaponHolderComponent::SwitchWeapon(int32 WeaponIndex)
{
	HideActiveWeapon();
	ActiveWeaponIndex = WeaponIndex;
	PickWeapon();
}

void UWeaponHolderComponent::NextWeapon_Implementation()
{
	if(!Weapons.Num() == 0 && ActiveWeaponIndex + 1 < Weapons.Num())
	{
		ActiveWeaponIndex += 1;
		SwitchWeapon(ActiveWeaponIndex);
	}
}

void UWeaponHolderComponent::PreviewWeapon_Implementation()
{
	//Need add to cycle swtich like 0 - 1 = last weapon, but later
	if(!Weapons.Num() == 0 && ActiveWeaponIndex - 1 >= 0)
	{
		ActiveWeaponIndex -= 1;
		SwitchWeapon(ActiveWeaponIndex);
	}
}

ABaseWeapon& UWeaponHolderComponent::GetActiveWeapon()
{
	return *Weapons[ActiveWeaponIndex];
}

void UWeaponHolderComponent::DropAllWeapon_Implementation()
{
	for (const TObjectPtr<ABaseWeapon> Weapon : Weapons)
	{
		Weapon->SetActorHiddenInGame(false);
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->SK_Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Weapon->SK_Weapon->SetSimulatePhysics(true);
	}
}

bool UWeaponHolderComponent::IsEmpty()
{
	return ActiveWeaponIndex == -1;
}

void UWeaponHolderComponent::BeginPlay()
{
	Super::BeginPlay();
	if(!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	}
}

