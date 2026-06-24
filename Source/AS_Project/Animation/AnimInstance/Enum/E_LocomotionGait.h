#pragma once

#include "CoreMinimal.h"
#include "E_LocomotionGait.generated.h"

UENUM(BlueprintType)
enum class E_LocomotionGait : uint8
{
    Walk    UMETA(DisplayName = "Walk"),
    Run     UMETA(DisplayName = "Run"),
    Sprint  UMETA(DisplayName = "Sprint"),
    None    UMETA(DisplayName = "None"),
}; 