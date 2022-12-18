// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TestProject/ProjectBaseController.h"
#include "BaseCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UWeaponHolderComponent;
class UHealthComponent;
class UDamageComponent;

UCLASS(config=Game)
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Actor Setup")
	TObjectPtr<UBillboardComponent> BackWeaponPlaceholder;
	UPROPERTY(EditAnywhere, Category = "Actor Setup")
	TObjectPtr<UBillboardComponent> WeaponToFirePlaceholder;
#endif
	
	UPROPERTY(EditAnywhere, Category = "Playble Components")
	TObjectPtr<UWeaponHolderComponent> WeaponHolderComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playble Components")
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(EditAnywhere, Category = "Playble Components")
	TObjectPtr<UDamageComponent> DamageComponent;

	FTransform WeaponBackTransform;
	FTransform WeaponToFireTransform;

public:
	ABaseCharacter();
	
	UWeaponHolderComponent& GetWeaponHolderComponent() const;
	const FTransform& GetBackWeaponTransform() const;
	const FTransform& GetWeaponToFireTransform() const;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Die();
	void OnFire();
	void SwitchWeaponUp();
	void SwitchWeaponDown();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
