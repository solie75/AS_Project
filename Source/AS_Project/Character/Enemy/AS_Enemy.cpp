// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_Enemy.h"


// Sets default values
AAS_Enemy::AAS_Enemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAS_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAS_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAS_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

