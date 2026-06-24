// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_GameMode.h"
#include "Character/Player/AS_Player.h"
#include "Core/PlayerController/AS_PlayerController.h"

AAS_GameMode::AAS_GameMode()
{
	DefaultPawnClass = AAS_Player::StaticClass();
	PlayerControllerClass = AAS_PlayerController::StaticClass();
}
