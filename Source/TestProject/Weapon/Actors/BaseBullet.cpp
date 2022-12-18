// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TestProject/Character/Components/DamageComponent.h"

ABaseBullet::ABaseBullet() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("BaseBullet");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->OnComponentHit.AddDynamic(this, &ABaseBullet::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

	InitialLifeSpan = 3.0f;
}

void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseBullet::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABaseBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
#if WITH_EDITOR
void ABaseBullet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ProjectileMovement->InitialSpeed = BulletData.Speed;
	ProjectileMovement->MaxSpeed = BulletData.Speed;

}
#endif

void ABaseBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		if(const TObjectPtr<UDamageComponent> OtherDamageComponent = OtherActor->FindComponentByClass<UDamageComponent>())
		{
			OtherDamageComponent->ApplyDamage(BulletData.Damage);
		}
		
		Destroy();
	}
}
