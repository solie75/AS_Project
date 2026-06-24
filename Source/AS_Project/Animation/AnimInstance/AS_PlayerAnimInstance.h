// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AS_BaseAnimInstance.h"
#include "AS_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AS_PROJECT_API UAS_PlayerAnimInstance : public UAS_BaseAnimInstance
{
	GENERATED_BODY()

	// Update
public:
	virtual void UpdatePropertiesFromCharacter() override;
};
