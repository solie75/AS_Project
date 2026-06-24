#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AS_FoleyAudioEntry.generated.h"

USTRUCT(BlueprintType)
struct FAS_FoleyAudioEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Event;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound;
};
