#pragma once
#include "CoreMinimal.h"
#include "E_MovementDirection.generated.h"

UENUM(BlueprintType)
enum class E_MovementDirection : uint8
{
    F   UMETA(DisplayName = "Forward"),
    B   UMETA(DisplayName = "Backward"),
    LL  UMETA(DisplayName = "Left - Left Foot Forward"),
    LR  UMETA(DisplayName = "Left - Right Foot Forward"),
    RL  UMETA(DisplayName = "Right - Left Foot Forward"),
    RR  UMETA(DisplayName = "Right - Right Foot Forward")
};