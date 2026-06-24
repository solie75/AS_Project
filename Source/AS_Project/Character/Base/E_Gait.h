// E_Gait.h
#pragma once

#include "CoreMinimal.h"
#include "E_Gait.generated.h"

UENUM(BlueprintType)
enum class E_Gait : uint8
{
	Walk   UMETA(DisplayName = "Walk"),
	Run    UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint")
}; 