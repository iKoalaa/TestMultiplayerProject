// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UArrowComponent;
UCLASS()
class ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Actor Settings")
	TObjectPtr<USkeletalMeshComponent> SK_Weapon;
	UPROPERTY(EditAnywhere, Category = "Actor Settings")
	TSubclassOf<ABaseBullet> BaseBulletSubclass;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	TObjectPtr<USoundBase> FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UAnimMontage> FireAnimation;

private:
	FTransform MuzzleTransform;
	
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:

	UFUNCTION(Server, Reliable)
	void Fire();
	void Fire_Implementation();

	TObjectPtr<UAnimMontage> GetFireAnimation();
	virtual void Tick(float DeltaTime) override;
};
