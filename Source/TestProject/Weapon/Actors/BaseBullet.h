
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UDamageComponent;
class USphereComponent;
class UProjectileMovementComponent;

USTRUCT()
struct FBulletData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float Speed;
};

UCLASS(config=Game)
class ABaseBullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	TObjectPtr<USphereComponent> CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
	UPROPERTY(EditAnywhere, Category = "BulletData")
	FBulletData BulletData;
	
public:
	UPROPERTY(EditAnywhere, Category = "BulletData")
	TObjectPtr<UDamageComponent> DamageComponent;
	
public:
	ABaseBullet();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	TObjectPtr<USphereComponent> GetCollisionComp() const { return CollisionComp; }
	TObjectPtr<UProjectileMovementComponent> GetProjectileMovement() const { return ProjectileMovement; }
};

