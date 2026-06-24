// FAS_CharacterAnimProperties.h
#pragma once

#include "CoreMinimal.h"
#include "Enum/AS_AnimInstanceTypes.h"
#include "Character/Player/FAS_PlayerInputState.h"
#include "FAS_CharacterAnimProperties.generated.h"

USTRUCT(BlueprintType)
struct FAS_CharacterAnimProperties
{
    GENERATED_BODY()

    // ── Input State ────────────────────────────
    UPROPERTY(BlueprintReadOnly)
    FAS_PlayerInputState InputState;

    UPROPERTY(BlueprintReadOnly)
    E_MovementMode MovementMode = E_MovementMode::OnGround;

    UPROPERTY(BlueprintReadOnly)
    E_Stance Stance = E_Stance::Stand;

    UPROPERTY(BlueprintReadOnly)
    E_RotationMode RotationMode = E_RotationMode::OrientToMovement;

    UPROPERTY(BlueprintReadOnly)
    E_Gait Gait = E_Gait::Walk;

    UPROPERTY(BlueprintReadOnly)
    E_MovementDirection MovementDirection = E_MovementDirection::F;

    // ── Transform ──────────────────────────────
    UPROPERTY(BlueprintReadOnly)
    FTransform ActorTransform;

    // ── Velocity / Acceleration ─────────────────
    UPROPERTY(BlueprintReadOnly)
    FVector Velocity = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    FVector InputAcceleration = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    float CurrentMaxAcceleration = 0.f;

    UPROPERTY(BlueprintReadOnly)
    float CurrentMaxDeceleration = 0.f;

    // ── Rotation ────────────────────────────────
    UPROPERTY(BlueprintReadOnly)
    FRotator OrientationIntent = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadOnly)
    FRotator AimingRotation = FRotator::ZeroRotator;

    // ── Landing ─────────────────────────────────
    UPROPERTY(BlueprintReadOnly)
    bool bJustLanded = false;

    UPROPERTY(BlueprintReadOnly)
    FVector LandVelocity = FVector::ZeroVector;

    // ── Steering ────────────────────────────────
    UPROPERTY(BlueprintReadOnly)
    float SteeringTime = 0.f;

    // ── Ground ──────────────────────────────────
    /*UPROPERTY(BlueprintReadOnly)
    FVector GroundNormal = FVector::UpVector;*/ // mover

    UPROPERTY(BlueprintReadOnly)
    FVector GroundLocation = FVector::ZeroVector;
};