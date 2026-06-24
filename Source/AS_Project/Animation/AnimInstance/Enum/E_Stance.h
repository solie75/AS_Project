#pragma once

#include "CoreMinimal.h"
#include "E_Stance.generated.h"

UENUM(BlueprintType)
enum class E_Stance : uint8
{
	Stand    UMETA(DisplayName = "Stand"),
	Crouch   UMETA(DisplayName = "Crouch")
}; 