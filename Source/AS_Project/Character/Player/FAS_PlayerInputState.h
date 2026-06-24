#pragma once

#include "CoreMinimal.h"
#include "FAS_PlayerInputState.generated.h"

USTRUCT(BlueprintType)
struct FAS_PlayerInputState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWantsToSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWantsToWalk = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWantsToStrafe = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWantsToAim = false;
};