#pragma once

#include "CoreMinimal.h"
#include "E_LandType.generated.h"

UENUM(BlueprintType)
enum class E_LandType : uint8
{
	None	UMETA(DisplayName = "None"),
	Light    UMETA(DisplayName = "Light"),
	Heavy     UMETA(DisplayName = "Heavy"),
}; 