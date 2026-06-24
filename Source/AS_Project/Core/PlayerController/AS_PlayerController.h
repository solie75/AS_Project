// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AS_PlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AS_PROJECT_API AAS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAS_PlayerController();
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:
	virtual void BeginPlay() override;
};
