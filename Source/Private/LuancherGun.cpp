// Fill out your copyright notice in the Description page of Project Settings.


#include "LuancherGun.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALuancherGun::ALuancherGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SetRootComponent(Mesh);

	
}

// Called when the game starts or when spawned
void ALuancherGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALuancherGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

