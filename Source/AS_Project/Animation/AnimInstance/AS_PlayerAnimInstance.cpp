// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_PlayerAnimInstance.h"
#include "Character/Interface/AS_CharacterInterface.h"

void UAS_PlayerAnimInstance::UpdatePropertiesFromCharacter()
{
	const AActor* Owner = GetOwningActor();
	if (!Owner) return;

	if (!Owner->Implements<UAS_CharacterInterface>()) return;

	CharacterProperties = 
		IAS_CharacterInterface::Execute_Get_PropertiesForAnimation(Owner);
}