#pragma once
#include "CoreMinimal.h"
#include "E_RotationMode.generated.h"

UENUM(BlueprintType)
enum class E_RotationMode : uint8
{
	OrientToMovement    UMETA(DisplayName = "Orient To Movement"),
	Strafe              UMETA(DisplayName = "Strafe"),
	Aim                 UMETA(DisplayName = "Aim")
};