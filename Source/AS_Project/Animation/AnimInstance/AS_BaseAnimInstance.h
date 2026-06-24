// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FAS_CharacterAnimProperties.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "Animation/TrajectoryTypes.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "AS_BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AS_PROJECT_API UAS_BaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAS_BaseAnimInstance();

	// Native
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	// AnimInstance Base
private:
	TArray<FName> CurrentDatabaseTags;

public:
	UFUNCTION(BlueprintCallable,  meta = (BlueprintThreadSafe), Category = "Database")
	TArray<FName> GetCurrentDatabaseTags() const;
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Database")
	void SetCurrentDatabaseTags(const TArray<FName>& InputTags);

	// Update
protected:
	bool bHasOwningActor = false;
	
	UFUNCTION()
	virtual void UpdateCVarDrivenVariables();

	UFUNCTION()
	virtual void UpdatePropertiesFromCharacter();

	UFUNCTION()
	virtual void UpdateTrajectory();

	UFUNCTION()
	virtual void UpdateEssentialValues();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateOffsetRootboneTransform();
	
	UFUNCTION()
	virtual void UpdateState();

	UFUNCTION()
	virtual void UpdateLogic();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	virtual EPoseSearchInterruptMode Get_MMInterruptMode() const;

	// SJE
	/*UPROPERTY(BlueprintReadOnly, Category = "CVar")
	bool bOffsetRootBoneEnabled = false;*/

	UPROPERTY(BlueprintReadOnly, Category = "CVar")
	float OffsetRootTranslationRadius = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "CVar")
	bool bDebugDrawShapes = false;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Value")
	FAS_CharacterAnimProperties CharacterProperties;

private:
	// CVar 포인터 캐시 (이름 탐색은 초기 1회만)
	//IConsoleVariable* CVar_OffsetRootBoneEnabled      = nullptr;
	IConsoleVariable* CVar_OffsetRootTranslationRadius = nullptr;
	IConsoleVariable* CVar_DrawDebugShapes            = nullptr;

	//====== UPDATE LOGIC =======
	
	// Trajectory
protected:
	UPROPERTY(EditAnywhere, Category = "Trajectory")
	FPoseSearchTrajectoryData TrajectoryGenerationDataIdle;

	UPROPERTY(EditAnywhere, Category = "Trajectory")
	FPoseSearchTrajectoryData TrajectoryGenerationDataMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	FTransformTrajectory Trajectory;

	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;

	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	FVector TrajPastVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	FVector TrajCurrentVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly,Category = "Trajectory")
	FVector TrajFutureVelocity = FVector::ZeroVector;

	// Essential Value
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite,Category = "Essential Values")
	float Speed2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Values")
	FTransform CharacterTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	FTransform RootTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	FVector Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	float AccelerationAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	FVector VelocityAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	FVector LastNonZeroVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	float HeavyLandSpeedThreshold = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Essential Value")
	bool JustLanded = false;

	UFUNCTION(BlueprintCallable, Category = "Essential Value")
	void SetJustLanded(bool InJustLanded);

	// State
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_MovementMode MovementModeLastFrame;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_MovementMode MovementMode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_RotationMode RotationModeLastFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "State")
	E_RotationMode RotationMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "State")
	E_MovementState MovementStateLastFrame;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_MovementState MovementState;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_Gait GaitLastFrame;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_Gait Gait;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	E_Stance StanceLastFrame;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "State")
	E_Stance Stance;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	bool IsMoving() const;

	// Motion Matching
protected:
	UFUNCTION(BlueprintPure, Category = "MotionMatching")
	float Get_MMBlendTime() const;

	UFUNCTION(BlueprintPure, Category = "MotionMatching")
	float Get_MMNotifyRecencyTimeOut() const;
	
private:
	float PreviousDesiredControllerYaw = 0.0f;

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Movement Analysis | Land")
	bool IsLandLight() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Movement Analysis | Land")
	bool IsLandHeavy() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Analysis | Land")
	FVector LandVelocity;
	
	UFUNCTION(BlueprintPure,meta = (BlueprintThreadSafe), Category = "Movement Analysis | Turn")
	bool ShouldTurnInPlace() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Movement Analysis | Gait")
	bool IsStarting() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Trajectory")
	float Get_TrajectoryTurnAngle() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Traversal")
	bool JustTraversal() const;
	
	// Pivot
public:
	UPROPERTY(EditDefaultsOnly, Category = "Pivot Settings")
	float PivotThreshold_OrientToMovement = 45.f;

	UPROPERTY(EditDefaultsOnly, Category = "Pivot Settings")
	float PivotThreshold_Strafe = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Pivot Settings")
	float PivotThreshold_Aim = 0.f;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Movement Analysis | Pivot")
	bool IsPivoting() const;

	// spin
public:
	UPROPERTY(EditDefaultsOnly, Category = "Spin Transition Settings")
	float SpinTransitionAngleThreshold = 130.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spin Transition Settings")
	float SpinTransitionSpeedThreshold = 150.f;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Movement Analysis | Spin")
	bool ShouldSpinTransition() const;

	// Lean
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Lean")
	FVector2D CalculateRelativeAccelerationAmount();

	// Aim Offset
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "AO")
	FVector2D Get_AOValue();	
};