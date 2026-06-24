// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_PreCMCTickComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreCMCTick);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AS_PROJECT_API UAS_PreCMCTickComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAS_PreCMCTickComponent();

	// Dispatch
	UPROPERTY(BlueprintAssignable)
	FOnPreCMCTick OnPreCMCTick;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
