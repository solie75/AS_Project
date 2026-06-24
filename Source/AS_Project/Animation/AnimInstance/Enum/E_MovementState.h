#pragma once

#include "CoreMinimal.h"
#include "E_MovementState.generated.h"

UENUM(BlueprintType)
enum class E_MovementState : uint8
{
	Idle    UMETA(DisplayName = "Idle"),
	Move     UMETA(DisplayName = "Move")
}; 