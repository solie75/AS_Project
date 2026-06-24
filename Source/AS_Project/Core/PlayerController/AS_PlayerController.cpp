// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_PlayerController.h"
#include "EnhancedInputSubsystems.h"

AAS_PlayerController::AAS_PlayerController()
{
	
}

void AAS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
