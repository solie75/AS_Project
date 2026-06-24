// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Components/AS_FoleyAudioEntry.h"
#include "AS_FoleyAudioBank.generated.h"

/**
 * 
 */
UCLASS()
class AS_PROJECT_API UAS_FoleyAudioBank : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foley")
	TArray<FAS_FoleyAudioEntry> Assets;

	UFUNCTION(BlueprintCallable, Category = "Foley")
	bool GetSoundFromFoleyEvent(FGameplayTag Event, USoundBase*& OutSound);
};
