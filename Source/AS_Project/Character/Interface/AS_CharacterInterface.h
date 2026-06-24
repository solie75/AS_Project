// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Animation/AnimInstance/FAS_CharacterAnimProperties.h"
#include "Character/Player/FAS_PlayerInputState.h"
#include "AS_CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAS_CharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AS_PROJECT_API IAS_CharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Interface")
	void Set_CharacterInputState(const FAS_PlayerInputState InputState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Interface")
	FAS_CharacterAnimProperties Get_PropertiesForAnimation() const;
};
