// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGun::StaticClass(), FoundGuns);
	if (FoundGuns.Num() > 0)
	{
		Gun = Cast<AGun>(FoundGuns[0]);
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ACharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShooterCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &AShooterCharacter::PickUp);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApplied = FMath::Min(Health, DamageToApplied);
	Health -= DamageToApplied;
	UE_LOG(LogTemp, Display, TEXT("Health left: %f"), Health);
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("You Died!"));
	}
	
	return DamageAmount;
}


bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

void AShooterCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AShooterCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AShooterCharacter::Fire()
{
	if (Gun)
	{
		Gun->PullTrigger();
	}
	
}

void AShooterCharacter::PickUp()
{
	if (HasGun && Gun)
	{
		Gun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Gun->FindComponentByClass<USkeletalMeshComponent>()->SetSimulatePhysics(true);
		Gun->FindComponentByClass<USkeletalMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Gun->SetOwner(nullptr);
		HasGun = false;
	}
	else if (Gun)
	{
		FVector GunLocation = Gun->GetActorLocation();
		FVector CharacterLocation = GetActorLocation();
		float Distance = FVector::Dist(GunLocation, CharacterLocation);
		if (Distance <= 200.f)
		{
			Gun->FindComponentByClass<USkeletalMeshComponent>()->SetSimulatePhysics(false);
			Gun->FindComponentByClass<USkeletalMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_Socket1"));
			Gun->SetOwner(this);
			HasGun = true;
		}
	}
}

