#pragma once

#include "CoreMinimal.h"
#include "AS_FoleyTypes.generated.h"

USTRUCT(BlueprintType)
struct FAS_FoleyEventParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Foley")
	float Side = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Foley")
	float Volume = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Foley")
	float Pitch = 1.0f;
};