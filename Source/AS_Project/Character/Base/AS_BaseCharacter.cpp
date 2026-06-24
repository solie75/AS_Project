// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_BaseCharacter.h"
#include "AS_CustomCharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "KismetAnimationLibrary.h"
#include "Components/AS_FoleyAudioEntry.h"
#include "GameFramework/CharacterMovementComponent.h"

AAS_BaseCharacter::AAS_BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UAS_CustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	PreCMCTickComponent = CreateDefaultSubobject<UAS_PreCMCTickComponent>(TEXT("PreCMCTick"));

	// Movement Value Init
	WalkSpeed = FVector(200.f, 180.f, 150.f);
	RunSpeed = FVector(500.f, 350.f, 300.f);
	SprintSpeed = FVector(700.f, 700.f, 700.f);
	CrouchSpeed = FVector(225.f, 200.f, 180.f);

	// Audio Component Init
	FoleyAudioComponent = CreateDefaultSubobject<UAS_Character_FoleyAudioComponent>(TEXT("Character_FoleyAudioComponent"));
	FoleyAudioComponent->SetupAttachment(RootComponent);
	FoleyAudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AAS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set CustomCMC Ptr
	CustomCMC = Cast<UAS_CustomCharacterMovementComponent>(GetCharacterMovement());

	// CMC 보다 PreCMCTick이 먼저 실행되도록 순서를 보장한다.
	CustomCMC->AddTickPrerequisiteComponent(PreCMCTickComponent);

	PreCMCTickComponent->OnPreCMCTick.AddDynamic(
		this,
		&AAS_BaseCharacter::PreCMCTick
	);
}

void AAS_BaseCharacter::PreCMCTick()
{
	UpdateRotation_PreCMC();
	UpdateMovement_PreCMC();
}


void AAS_BaseCharacter::UpdateRotation_PreCMC()
{
	
}

void AAS_BaseCharacter::UpdateMovement_PreCMC()
{
	// 물리값 업데이트
	CustomCMC->MaxAcceleration            = CalculateMaxAcceleration();
	CustomCMC->BrakingDecelerationWalking = CalculateBrakingDeceleration();
	CustomCMC->GroundFriction             = CalculateGroundFriction();

	// 속도 업데이트
	CustomCMC->MaxWalkSpeed         = CalculateMaxSpeed();
	CustomCMC->MaxWalkSpeedCrouched = CalculateMaxCrouchSpeed();
}

E_Gait AAS_BaseCharacter::GetDesiredGait()
{
	return E_Gait::Run;
}

bool AAS_BaseCharacter::CanSprint()
{
	return false;
}

float AAS_BaseCharacter::CalculateMaxAcceleration()
{
	return CalculateSprintMappedValue(800.f, 300.f, 700.f, 800.f, 300.f);
}

float AAS_BaseCharacter::CalculateMaxSpeed()
{
	FVector Speed = FVector::ZeroVector;
	switch (CurGait)
	{
		case E_Gait::Walk : Speed = WalkSpeed; break;
		case E_Gait::Run : Speed = RunSpeed; break;
		case E_Gait::Sprint : Speed = SprintSpeed; break;
		default : Speed = RunSpeed; break;
	}
	
	return CalculateSpeedByDirection(Speed);
}

float AAS_BaseCharacter::CalculateSpeedByDirection(const FVector& Speeds)
{
	const float Direction    = UKismetAnimationLibrary::CalculateDirection(
								   CustomCMC->Velocity, GetActorRotation());
	const float AbsDirection = FMath::Abs(Direction);

	const float CurveValue   = StrafeSpeedMapCurve
		? StrafeSpeedMapCurve->GetFloatValue(AbsDirection) : 0.f;

	const float StrafeSpeedMap = CustomCMC->bOrientRotationToMovement
		? 0.f : CurveValue;

	if (StrafeSpeedMap < 1.f)
		return FMath::GetMappedRangeValueClamped(
			FVector2D(0.f, 1.f), FVector2D(Speeds.X, Speeds.Y), StrafeSpeedMap);
	else
		return FMath::GetMappedRangeValueClamped(
			FVector2D(1.f, 2.f), FVector2D(Speeds.Y, Speeds.Z), StrafeSpeedMap);
}

float AAS_BaseCharacter::CalculateBrakingDeceleration()
{
	return GetPendingMovementInputVector().IsNearlyZero() ? 2000.f : 500.f;
}

float AAS_BaseCharacter::CalculateMaxCrouchSpeed()
{
	return CalculateSpeedByDirection(CrouchSpeed);
}

float AAS_BaseCharacter::CalculateGroundFriction()
{
	return CalculateSprintMappedValue(5.f, 0.f, 500.f, 5.f, 3.f);
}

float AAS_BaseCharacter::CalculateSprintMappedValue(float FixedValue, float InRangeA, float InRangeB, float OutRangeA,
	float OutRangeB)
{
	const float Speed = FVector2D(CustomCMC->Velocity.X, CustomCMC->Velocity.Y).Length();

	switch (CurGait)
	{
	case E_Gait::Walk:
	case E_Gait::Run:
		return FixedValue;
	case E_Gait::Sprint:
		return FMath::GetMappedRangeValueClamped(
			FVector2D(InRangeA, InRangeB),
			FVector2D(OutRangeA, OutRangeB),
			Speed
		);
	default:
		return FixedValue;
	}
}

void AAS_BaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bJustLanded = true;
	LandVelocity = GetVelocity();

	GetWorldTimerManager().SetTimer(
		LandedTimer,
		[this](){bJustLanded = false;},
		0.3f,
		false
		);

	FAS_FoleyEventParams Params;
	Params.Volume = FMath::GetMappedRangeValueClamped(
		FVector2D(-500.f, -900.f),
		FVector2D(0.5f, 1.5f),
		LandVelocity.Z
		);
	
	BP_OnLanded(Params);   
}

bool AAS_BaseCharacter::Get_bJustLanded() const
{
	return bJustLanded;
}

FVector AAS_BaseCharacter::Get_LandVelocity() const
{
	return LandVelocity;
}

void AAS_BaseCharacter::SetGait(E_Gait NewGait)
{
	CurGait = NewGait;
}

void AAS_BaseCharacter::Set_CharacterInputState_Implementation(const FAS_PlayerInputState InputState)
{
	// Player 이외의 Base Character 의 파생 클래스에서 링크 에러가 발생하지 않도록 빈 함수 구현.
}

FAS_CharacterAnimProperties AAS_BaseCharacter::Get_PropertiesForAnimation_Implementation() const
{
	return IAS_CharacterInterface::Get_PropertiesForAnimation_Implementation();
}

// Called every frame
void AAS_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

