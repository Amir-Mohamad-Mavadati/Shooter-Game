// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(Mesh);
	
}

void AGun::PullTrigger()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	UGameplayStatics::SpawnEmitterAttached(MuzzelEffect, Mesh, TEXT("MuzzleFlashSocket"));
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	FVector EndLocation = Location + Rotation.Vector() * MaxRang;
	FHitResult Hit;
	bool IsHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, EndLocation, ECollisionChannel::ECC_GameTraceChannel1);
	if (IsHit)
	{
		if (HitEffect)
		{
			FVector ShootDirection = -Rotation.Vector();
			UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, Hit.Location, ShootDirection.Rotation(), true);
			AActor* HitActor = Hit.GetActor();
			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShootDirection, nullptr);
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
			
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

