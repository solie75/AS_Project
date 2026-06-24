// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_BaseAnimInstance.h"
#include "Character/Base/AS_BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"


#define READ_CVAR_BOOL(CVar, Member)  if (CVar) Member = CVar->GetBool()
#define READ_CVAR_FLOAT(CVar, Member) if (CVar) Member = CVar->GetFloat()

UAS_BaseAnimInstance::UAS_BaseAnimInstance()
{
}

void UAS_BaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Set CVar
	IConsoleManager& CM = IConsoleManager::Get();

	//CVar_OffsetRootBoneEnabled       = CM.FindConsoleVariable(TEXT("AS.OffsetRootBone.Enable"));
	CVar_OffsetRootTranslationRadius = CM.FindConsoleVariable(TEXT("AS.OffsetRootBone.TranslationRadius"));
	CVar_DrawDebugShapes             = CM.FindConsoleVariable(TEXT("AS.DrawCharacterDebugShapes"));
}

void UAS_BaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	bHasOwningActor = (TryGetPawnOwner() != nullptr);
	if (!bHasOwningActor)
	{
		return;
	}

	UpdateCVarDrivenVariables();
	UpdatePropertiesFromCharacter();
	UpdateLogic();
}

void UAS_BaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!bHasOwningActor)
	{
		return;
	}
	
	//UpdateLogic();
}

TArray<FName> UAS_BaseAnimInstance::GetCurrentDatabaseTags() const
{
	return CurrentDatabaseTags;
}

void UAS_BaseAnimInstance::SetCurrentDatabaseTags(const TArray<FName>& InputTags)
{
	CurrentDatabaseTags = InputTags;
}

void UAS_BaseAnimInstance::UpdateCVarDrivenVariables()
{
	//READ_CVAR_BOOL (CVar_OffsetRootBoneEnabled,       bOffsetRootBoneEnabled);
	READ_CVAR_FLOAT(CVar_OffsetRootTranslationRadius, OffsetRootTranslationRadius);
	READ_CVAR_BOOL (CVar_DrawDebugShapes,             bDebugDrawShapes);
}

void UAS_BaseAnimInstance::UpdatePropertiesFromCharacter()
{
	// 파생 함수에서 구현
}

void UAS_BaseAnimInstance::UpdateTrajectory()
{
	const FPoseSearchTrajectoryData TrajectoryData = (Speed2D > 0.f)
	? TrajectoryGenerationDataMoving
	: TrajectoryGenerationDataIdle;

	FTransformTrajectory OutTrajectory;
	UPoseSearchTrajectoryLibrary::PoseSearchGenerateTransformTrajectory(
		this,
		TrajectoryData,
		GetDeltaSeconds(),
		Trajectory,
		PreviousDesiredControllerYaw,
		OutTrajectory,
		-1.f,
		30,
		0.1f,
		15
		);

	Trajectory = OutTrajectory;

	UPoseSearchTrajectoryLibrary::HandleTransformTrajectoryWorldCollisions(
	   this, this,
	   Trajectory,
	   true,
	   0.01f,
	   OutTrajectory,
	   TrajectoryCollision,
	   UEngineTypes::ConvertToTraceType(ECC_Visibility),
	   false,
	   {},
	   EDrawDebugTrace::None,
	   true,
	   150.0f,
	   FLinearColor::Red,
	   FLinearColor::Green,
	   5.0f
   );

	Trajectory = OutTrajectory;

	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory, -0.3f, -0.2f, TrajPastVelocity, false);
	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory, 0.f, 0.2f, TrajCurrentVelocity, false);
	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory, 0.4f, 0.5f, TrajFutureVelocity, false);
}

void UAS_BaseAnimInstance::UpdateEssentialValues()
{
	// Character Transform
	CharacterTransform = CharacterProperties.ActorTransform;

	// Root Transform
	UpdateOffsetRootboneTransform();

	// SJE
	/*if (!bOffsetRootBoneEnabled)
	{
		/*const FVector Location = CharacterTransform.GetLocation();
		const FRotator Rotation = CharacterTransform.Rotator();

		RootTransform = FTransform(
			FQuat(FRotator(Rotation.Pitch, Rotation.Yaw + 90.f, Rotation.Roll)),
			Location,
			FVector::OneVector
			);#1#

		RootTransform = CharacterTransform;
	}
	else
	{
		UpdateOffsetRootboneTransform();
	}*/
	
	// Acceleration
	Acceleration = CharacterProperties.InputAcceleration;
	const float MaxAcc = FMath::Max(CharacterProperties.CurrentMaxAcceleration, 1.f);
	AccelerationAmount = FMath::Clamp(Acceleration.Size() / MaxAcc, 0.f, 1.f);

	// Velocity
	FVector VelocityLastFrame = Velocity;
	Velocity = CharacterProperties.Velocity;
	Speed2D = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	if (Speed2D > 0.f)
	{
		LastNonZeroVelocity = Velocity.GetSafeNormal();
	}

	const float DeltaSeconds = GetDeltaSeconds();
	if (DeltaSeconds > SMALL_NUMBER)
	{
		VelocityAcceleration = (Velocity - VelocityLastFrame) / DeltaSeconds;
	}

	// Land
	LandVelocity = CharacterProperties.LandVelocity;
	JustLanded = CharacterProperties.bJustLanded;
}

void UAS_BaseAnimInstance::UpdateOffsetRootboneTransform_Implementation()
{
	// ...
}

void UAS_BaseAnimInstance::UpdateState()
{
	MovementModeLastFrame = MovementMode;
	MovementMode = CharacterProperties.MovementMode;
	RotationModeLastFrame = RotationMode;
	RotationMode = CharacterProperties.RotationMode;
	MovementStateLastFrame = MovementState;
	if (IsMoving())
	{
		MovementState = E_MovementState::Move;
	}
	else
	{
		MovementState = E_MovementState::Idle;
	}
	GaitLastFrame = Gait;
	Gait = CharacterProperties.Gait;
	StanceLastFrame = Stance;
	Stance = CharacterProperties.Stance;
}

void UAS_BaseAnimInstance::UpdateLogic()
{
	UpdateEssentialValues();
	UpdateTrajectory();
	UpdateState();
}

EPoseSearchInterruptMode UAS_BaseAnimInstance::Get_MMInterruptMode() const
{
	const bool bMovementModeChanged = (MovementMode != MovementModeLastFrame);
	const bool bGaitChangedWhileMoving = (Gait != GaitLastFrame) && (MovementState == E_MovementState::Move);
	const bool bStanceChanged = (Stance != StanceLastFrame);

	const bool bAnyStateChanged = bGaitChangedWhileMoving || bStanceChanged || bMovementModeChanged;
	const bool bIsOnGround = (MovementMode == E_MovementMode::OnGround);

	const bool bShouldInterrupt = bMovementModeChanged || (bAnyStateChanged && bIsOnGround);

	return bShouldInterrupt
		? EPoseSearchInterruptMode::InterruptOnDatabaseChange
		: EPoseSearchInterruptMode::DoNotInterrupt;
}

void UAS_BaseAnimInstance::SetJustLanded(bool InJustLanded)
{
	JustLanded = InJustLanded;
}

bool UAS_BaseAnimInstance::IsMoving() const
{
	const bool bHasCurrentVelocity = !Velocity.IsNearlyZero(0.1f);
	const bool bHasCurrentAcceleration = !Acceleration.IsNearlyZero(0.0f);
	return bHasCurrentVelocity && bHasCurrentAcceleration;
}

float UAS_BaseAnimInstance::Get_MMBlendTime() const
{
	switch (MovementMode)
	{
	case E_MovementMode::OnGround:
		// 이전 프레임 공중 -> 방금 착지 / 착지 애니메이션을 위한 짧은 블렌드 타입.
		if (MovementModeLastFrame == E_MovementMode::InAir)
		{
			return 0.2f;
		}
		return 0.5f;
	case E_MovementMode::InAir:
		// 이전 프레임 지상 -> 방금 점프 / 점프 애니메이션을 위한 짧은 블렌드 타임.
		if (Velocity.Z > 100.f)
		{
			return 0.15f;
		}
		return 0.5f;
	}
	return 0.5f;
}

float UAS_BaseAnimInstance::Get_MMNotifyRecencyTimeOut() const
{
	switch (Gait)
	{
	case E_Gait::Walk:   return 0.2f;
	case E_Gait::Run:    return 0.2f;
	case E_Gait::Sprint: return 0.16f;
	default:             return 0.2f;
	}
}

bool UAS_BaseAnimInstance::IsLandLight() const
{
	return (FMath::Abs(LandVelocity.Z) < HeavyLandSpeedThreshold) && JustLanded;
}

bool UAS_BaseAnimInstance::IsLandHeavy() const
{
	return (FMath::Abs(LandVelocity.Z) >= HeavyLandSpeedThreshold) && JustLanded;
}

bool UAS_BaseAnimInstance::ShouldTurnInPlace() const
{
	// 루트 본의 회전과 캐릭터 캡슐의 회전 차이를 비교하여 회전해야 하는지를 판단한다.
	// 회전 차가 50 도 이상이고 현재 조준 중 이면 제자리 회전 수행

	// 루트 본 회전과 캐릭터의 방향 의도(OrientationIntent) 사이의 델타 회전 계산
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(
		CharacterProperties.OrientationIntent,
		RootTransform.Rotator()
	);

	const bool bJustStopped =
		MovementState == E_MovementState::Idle
		&& MovementStateLastFrame == E_MovementState::Move;

	return (bJustStopped || CharacterProperties.InputState.bWantsToAim)
		&& FMath::Abs(DeltaRotation.Yaw) >= 50.0f;
}

bool UAS_BaseAnimInstance::IsStarting() const
{
	// Start 와 Pivot 의 움직임이 비슷하기 때문에 Tags 로 Pivot 만 따로 여과한다.
	if (CurrentDatabaseTags.Contains(FName("Pivots")))
	{
		return false;
	}

	const float FutureSpeedXY = FVector(TrajFutureVelocity.X, TrajFutureVelocity.Y, 0.f).Size();
	const float CurrentSpeedXY = FVector(TrajCurrentVelocity.X, TrajCurrentVelocity.Y, 0.f).Size();

	return IsMoving() && FutureSpeedXY >= CurrentSpeedXY + 100.f;
}

float UAS_BaseAnimInstance::Get_TrajectoryTurnAngle() const
{
	const FRotator AccelRotation = FRotationMatrix::MakeFromX(Acceleration).Rotator();
	const FRotator VelocityRotation = FRotationMatrix::MakeFromX(Velocity).Rotator();
	return UKismetMathLibrary::NormalizedDeltaRotator(AccelRotation, VelocityRotation).Yaw;
}

bool UAS_BaseAnimInstance::JustTraversal() const
{
	const bool bSlotNotActive = !IsSlotActive(FName("DefaultSlot"));
	const float MovingTraversalCurve = GetCurveValue(FName("MovingTraversal"));
	const bool bMovingTraversalActive = MovingTraversalCurve > 0.f;
	const bool bTurnAngleSmall = FMath::Abs(Get_TrajectoryTurnAngle()) <= 50.f;

	return bSlotNotActive && bMovingTraversalActive && bTurnAngleSmall;
}

bool UAS_BaseAnimInstance::IsPivoting() const
{
	// 캐릭터의 Future Trajectory 가 Current Trajectory 와 방향이 매우 다른지를 판별하여 캐릭터가 피벗 중인지를 판단한다.

	if (!IsMoving())
	{
		return false;
	}

	float Threshold = 0.f;
	switch (RotationMode)
	{
	case E_RotationMode::OrientToMovement: Threshold = PivotThreshold_OrientToMovement; break;
	case E_RotationMode::Strafe:           Threshold = PivotThreshold_Strafe;           break;
	case E_RotationMode::Aim:              Threshold = PivotThreshold_Aim;              break;
	}

	
	return FMath::Abs(Get_TrajectoryTurnAngle()) >= Threshold;
}

bool UAS_BaseAnimInstance::ShouldSpinTransition() const
{
	if (CurrentDatabaseTags.Contains(FName("Pivot")))
	{
		return false;
	}

	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(
		CharacterTransform.Rotator(),
		RootTransform.Rotator()
		);

	return FMath::Abs(Delta.Yaw) >= SpinTransitionAngleThreshold && Speed2D >= SpinTransitionSpeedThreshold;;
}

FVector2D UAS_BaseAnimInstance::CalculateRelativeAccelerationAmount()
{
	if (CharacterProperties.CurrentMaxAcceleration <= 0.f || CharacterProperties.CurrentMaxDeceleration <= 0.f)
	{
		return FVector2D::ZeroVector;
	}
	// 가속 vs 감속 판정
	const bool bIsAccelerating = FVector::DotProduct(Acceleration, Velocity) > 0.f;
	// 붐노 Max 가속도
	const float MaxAcc = bIsAccelerating ? CharacterProperties.CurrentMaxAcceleration : CharacterProperties.CurrentMaxDeceleration;
	// 정규화
	const FVector ClampedAcc = VelocityAcceleration.GetClampedToMaxSize(MaxAcc);
	const FVector NormalizedAcc = ClampedAcc / MaxAcc;
	// 월드 기준의 가속도 비율 벡터를 캐릭터의 로컬 좌표계로 역회전.
	const FVector LocalAcceleration = CharacterTransform.GetRotation().UnrotateVector(NormalizedAcc);
	// Animation Blend Space 용 X(Forward), Y(Right) 반환.
	return FVector2D(LocalAcceleration.X, LocalAcceleration.Y);
}

FVector2D UAS_BaseAnimInstance::Get_AOValue()
{
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(CharacterProperties.AimingRotation, RootTransform.Rotator());
	float AlphaValue = GetCurveValue(TEXT("Disable_AO"));
	const FVector TempVec(DeltaRot.Yaw, DeltaRot.Pitch, 0.0f);

	/*FVector LerpedVector = FMath::Lerp(TempVec, FVector::ZeroVector, AlphaValue);
	return FVector2D(LerpedVector.X, LerpedVector.Y);*/
	float TargetX = FMath::Lerp(DeltaRot.Yaw, 0.0f, AlphaValue);
	float TargetY = FMath::Lerp(DeltaRot.Pitch, 0.0f, AlphaValue);

	return FVector2D(TargetX, TargetY);
}
