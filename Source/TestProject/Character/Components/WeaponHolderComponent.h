// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponHolderComponent.generated.h"

class ABaseCharacter;
class ABaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UWeaponHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponHolderComponent();
	
	UFUNCTION(NetMulticast, Unreliable)
	void AddWeapon(ABaseWeapon* NewWeapon);
	void AddWeapon_Implementation(ABaseWeapon* NewWeapon);
	UFUNCTION(NetMulticast, Unreliable)
	void HideActiveWeapon();
	void HideActiveWeapon_Implementation();
	UFUNCTION(NetMulticast, Unreliable)
	void PickWeapon();
	void PickWeapon_Implementation();
	UFUNCTION(NetMulticast, Unreliable)
	void NextWeapon();
	void NextWeapon_Implementation();
	UFUNCTION(NetMulticast, Unreliable)
	void PreviewWeapon();
	void PreviewWeapon_Implementation();
	
	UFUNCTION(NetMulticast, Unreliable)
	void DropAllWeapon();
	void DropAllWeapon_Implementation();

	ABaseWeapon& GetActiveWeapon();
	bool IsEmpty();

protected:
	virtual void BeginPlay() override;

	void SwitchWeapon(int32 WeaponIndex);
	
private:
	
	//TODO: Do better, but later
	UPROPERTY()
	TObjectPtr<ABaseCharacter> OwnerCharacter;
	TArray<TObjectPtr<ABaseWeapon>> Weapons;
	int32 ActiveWeaponIndex = -1;
};
