// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TestProject/Character/Components/DamageComponent.h"
#include "TestProject/Character/Components/HealthComponent.h"
#include "TestProject/Character/Components/WeaponHolderComponent.h"
#include "TestProject/Weapon/Actors/BaseWeapon.h"
#include "EnhancedInputSubsystems.h"
#include "TestProject/ProjectGameMode.h"

ABaseCharacter::ABaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	WeaponHolderComponent = CreateDefaultSubobject<UWeaponHolderComponent>(TEXT("WeaponHolderComponents"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

#if WITH_EDITOR
	BackWeaponPlaceholder = CreateDefaultSubobject<UBillboardComponent>(TEXT("BackWeaponPlaceholder"));
	WeaponToFirePlaceholder = CreateDefaultSubobject<UBillboardComponent>(TEXT("WeaponToFirePlaceholder"));
#endif
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapBegin);
	HealthComponent->OnDie.AddDynamic(this, &ABaseCharacter::Die);
}

void ABaseCharacter::Destroyed()
{
	
#if !WITH_EDITOR
	GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseCharacter::OnOverlapBegin);
	HealthComponent->OnDie.RemoveDynamic(this, &ABaseCharacter::Die);
#endif

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHolderComponent->DropAllWeapon();
	
	Super::Destroyed();
}

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABaseCharacter::OnFire);
	}
}

void ABaseCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const TObjectPtr<ABaseWeapon> PickableWeapon = Cast<ABaseWeapon>(Other))
	{
		WeaponHolderComponent->AddWeapon(PickableWeapon);
	}
}

void ABaseCharacter::Die()
{
	if (const TObjectPtr<AProjectGameMode> GameMode = GetWorld()->GetAuthGameMode<AProjectGameMode>())
	{
		GameMode->Respawn(this, Controller);
	}
}

void ABaseCharacter::SwitchWeaponDown()
{
	if (WeaponHolderComponent->IsEmpty())
	{
		return;
	}

	WeaponHolderComponent->NextWeapon();
	WeaponHolderComponent->GetActiveWeapon();
}

UWeaponHolderComponent& ABaseCharacter::GetWeaponHolderComponent() const
{
	return *WeaponHolderComponent;
}

void ABaseCharacter::SwitchWeaponUp()
{
	if (WeaponHolderComponent->IsEmpty())
	{
		return;
	}
	WeaponHolderComponent->PreviewWeapon();
	WeaponHolderComponent->GetActiveWeapon();
}

const FTransform& ABaseCharacter::GetBackWeaponTransform() const
{
	return WeaponBackTransform;
}

const FTransform& ABaseCharacter::GetWeaponToFireTransform() const
{
	return WeaponToFireTransform;
}

#if WITH_EDITOR
void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (BackWeaponPlaceholder)
	{
		WeaponBackTransform = BackWeaponPlaceholder->GetComponentTransform();
	}
	if (WeaponToFirePlaceholder)
	{
		WeaponToFireTransform = WeaponToFirePlaceholder->GetComponentTransform();
	}
}
#endif

void ABaseCharacter::OnFire()
{
	if (WeaponHolderComponent && !WeaponHolderComponent->IsEmpty())
	{
		WeaponHolderComponent->GetActiveWeapon().Fire();
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
