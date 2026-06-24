#pragma once

#include "CoreMinimal.h"
#include "E_MovementMode.generated.h"

UENUM(BlueprintType)
enum class E_MovementMode : uint8
{
	OnGround	UMETA(DisplayName = "OnGround"),
	InAir	UMETA(DisplayName = "InAir")
}; 