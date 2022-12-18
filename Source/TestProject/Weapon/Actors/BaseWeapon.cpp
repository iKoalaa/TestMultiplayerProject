// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif
	
	SK_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SK_Weapon);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABaseWeapon::Fire_Implementation()
{
	if(BaseBulletSubclass)
	{
		if (const TObjectPtr<UWorld> World = GetWorld())
		{
			MuzzleTransform = SK_Weapon->GetSocketTransform(TEXT("Muzzle"));
			
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<ABaseBullet>(BaseBulletSubclass, MuzzleTransform.GetLocation(), MuzzleTransform.GetRotation().Rotator(), ActorSpawnParams);
		}

		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

TObjectPtr<UAnimMontage> ABaseWeapon::GetFireAnimation()
{
	return FireAnimation;
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
