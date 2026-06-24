// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_Player.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameframework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Base/AS_CustomCharacterMovementComponent.h"


// Sets default values
AAS_Player::AAS_Player(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	// LegacyCamera Setting
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength         = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch           = true;
	SpringArmComponent->bInheritYaw             = true;
	SpringArmComponent->bInheritRoll            = false;

	LegacyCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("LegacyCamera"));
	LegacyCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	LegacyCameraComponent->bUsePawnControlRotation = false;
	LegacyCameraComponent->SetAutoActivate(false);

	// Gameplay Camera Setting
	GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCamera"));
	GameplayCameraComponent->SetupAttachment(GetRootComponent());
	GameplayCameraComponent->SetAutoActivate(false);

	// Movement
	AnalogWalkRunThreshold = 0.7f;
	MovementStickMode = E_MovementStickMode::FixedSpeed_SingleGait;
}

// Called when the game starts or when spawned
void AAS_Player::BeginPlay()
{
	Super::BeginPlay();

	if (UAS_CustomCharacterMovementComponent* CMC = GetCustomCMC())
	{
		CMC->NavAgentProps.bCanCrouch = true;
	}
}

// Called every frame
void AAS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::OnJump);
		EIC->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::OnWalkToggle);
		EIC->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &ThisClass::OnStrafeToggle);
		EIC->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::OnSprintStart);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::OnSprintEnd);
		EIC->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::OnCrouchToggle);
	}
}

void AAS_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupCamera();
}

void AAS_Player::OnMove(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		AddMovementInput(FRotationMatrix(YawRot).GetUnitAxis(EAxis::X), Axis.Y);
		AddMovementInput(FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y), Axis.X);
	}
}

void AAS_Player::OnLook(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AAS_Player::OnJump(const FInputActionValue& Value)
{
	Jump();
}

void AAS_Player::OnWalkToggle(const FInputActionValue& Value)
{
	if (PlayerInputState.bWantsToSprint)
	{
		return;
	}
	PlayerInputState.bWantsToWalk = !PlayerInputState.bWantsToWalk;
}

void AAS_Player::OnStrafeToggle(const FInputActionValue& Value)
{
	PlayerInputState.bWantsToStrafe = !PlayerInputState.bWantsToStrafe;
}

void AAS_Player::OnSprintStart(const FInputActionValue& Value)
{
	if (GetCustomCMC()->IsCrouching())
	{
		return;
	}
	PlayerInputState.bWantsToWalk = false;
	PlayerInputState.bWantsToSprint = true;
}

void AAS_Player::OnSprintEnd(const FInputActionValue& Value)
{
	PlayerInputState.bWantsToSprint = false;
}

void AAS_Player::OnCrouchToggle(const FInputActionValue& Value)
{
	IsCrouched() ? UnCrouch() : Crouch();
}

void AAS_Player::SetupCamera()
{
	// Camera Branching
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("DDVar.NewGameplayCameraSystem.Enable"));

	bool bUseNewCameraSystem = CVar ? CVar->GetBool() : false;

	if (bUseNewCameraSystem)
	{
		if (LegacyCameraComponent)
		{
			LegacyCameraComponent->Deactivate();
		}
		if (GameplayCameraComponent)
		{
			GameplayCameraComponent->ActivateCameraForPlayerController(
				PC,
				true // bSetAsViewTarget
			);
		}
	}
	else
	{
		if (GameplayCameraComponent)
		{
			GameplayCameraComponent->Deactivate();
		}
		if (LegacyCameraComponent)
		{
			LegacyCameraComponent->Activate(false);
		}
	}

	PC->SetViewTargetWithBlend(
		this,
		0.0f,
		EViewTargetBlendFunction::VTBlend_Linear,
		0.0f,
		false
		);
}

void AAS_Player::UpdateRotation_PreCMC()
{
	Super::UpdateRotation_PreCMC();

	UAS_CustomCharacterMovementComponent* CMC = GetCustomCMC();

	// Wants to Strafe 또는 Wants to Aim 이면 Strafe 모드 설정.
	if (PlayerInputState.bWantsToStrafe || PlayerInputState.bWantsToAim)
	{
		CMC->bOrientRotationToMovement = false;
		CMC->bUseControllerDesiredRotation = true;
	}
	else
	{
		const bool bIsMoving = GetVelocity().SizeSquared2D() > SMALL_NUMBER;
		CMC->bOrientRotationToMovement = bIsMoving;
		CMC->bUseControllerDesiredRotation = false;
	}

	// 지상이면 즉시 회전, 공중이면 보간 회전
	if (CMC->IsFalling())
	{
		CMC->RotationRate = FRotator(0.0, 100.0f, 0.0);
	}
	else
	{
		CMC->RotationRate = FRotator(0.0, 200.0f, 0.0);
	}
}

void AAS_Player::UpdateMovement_PreCMC()
{
	// Gait 업데이트
	SetGait(GetDesiredGait());
	// 나머지 업데이트
	Super::UpdateMovement_PreCMC();
}

E_Gait AAS_Player::GetDesiredGait()
{
	Super::GetDesiredGait();

	// 입력 값의 임계치 초과 검사.
	const FVector Input = GetPendingMovementInputVector(); // 해당 틱에 AddMovementInput 으로 누적된 입력 벡터를 반환.
	const float InputLength = FVector2D(Input.X, Input.Y).Length();
	bool bFullMovementInput = false;
	
	switch (MovementStickMode)
	{
	case	E_MovementStickMode::FixedSpeed_SingleGait :
		bFullMovementInput = true;
		break;
	case E_MovementStickMode::FixedSpeed_WalkRun :
		bFullMovementInput = InputLength >= AnalogWalkRunThreshold;
		break;
	case E_MovementStickMode::VariableSpeed_SingleGait :
		bFullMovementInput = true;
		break;
	case E_MovementStickMode::VariableSpeed_WalkRun :
		bFullMovementInput = InputLength >= AnalogWalkRunThreshold;
		break;
	}

	if (CanSprint())
	{
		return bFullMovementInput ? E_Gait::Sprint : E_Gait::Run;
	}
	if (PlayerInputState.bWantsToWalk || !bFullMovementInput)
	{
		return E_Gait::Walk;
	}
	return E_Gait::Run;
}

bool AAS_Player::CanSprint()
{
	UAS_CustomCharacterMovementComponent* LocalCMC = GetCustomCMC();
	if (!LocalCMC || LocalCMC->IsCrouching())
	{
		return false;
	}

	const FVector InputVector = GetPendingMovementInputVector();
	const FRotator InputRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
	const FRotator ActorRotation = GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(InputRotation, ActorRotation);
	const float YawDelta = FMath::Abs(Delta.Yaw);

	const bool bCanSprintByDirection =  GetCustomCMC()->bOrientRotationToMovement || YawDelta < 50.0f;

	return bCanSprintByDirection && PlayerInputState.bWantsToSprint;
}

void AAS_Player::Set_CharacterInputState_Implementation(const FAS_PlayerInputState InputState)
{
	PlayerInputState = InputState;
}

FAS_CharacterAnimProperties AAS_Player::Get_PropertiesForAnimation_Implementation() const
{
	FAS_CharacterAnimProperties Props = Super::Get_PropertiesForAnimation_Implementation();

	UAS_CustomCharacterMovementComponent* CMC = GetCustomCMC();
	
	if (IsValid(CMC))
	{
		Props.InputState = PlayerInputState;
		switch (CMC->MovementMode)
		{
		case MOVE_Falling:
		case MOVE_Swimming:
			Props.MovementMode = E_MovementMode::InAir;
			break;
		case MOVE_None:
		case MOVE_Walking:
		case MOVE_NavWalking:
		case MOVE_Flying:
		case MOVE_Custom:
		default:
			Props.MovementMode = E_MovementMode::OnGround;
			break;
		}
		Props.Stance = CMC->IsCrouching() ? E_Stance::Crouch : E_Stance::Stand;
		// SJE
		// UE_LOG(LogTemp, Warning, TEXT("Get_PropertiesForAnimation_Implementation : %s"), Props.Stance == E_Stance::Crouch ? TEXT("Crouch") : TEXT("Stand"));
		Props.RotationMode = CMC->bOrientRotationToMovement ? E_RotationMode::OrientToMovement : E_RotationMode::Strafe;
		Props.Gait = CurGait;
		Props.ActorTransform = GetActorTransform();
		Props.Velocity = CMC->Velocity;
		Props.InputAcceleration = CMC->GetCurrentAcceleration();
		Props.CurrentMaxAcceleration = CMC->GetMaxAcceleration();
		Props.CurrentMaxDeceleration = CMC->BrakingDecelerationWalking;
		Props.OrientationIntent = GetActorRotation();
		Props.AimingRotation = IsLocallyControlled() ? GetBaseAimRotation() : GetControlRotation();
		Props.bJustLanded = Get_bJustLanded();
		Props.LandVelocity = Get_LandVelocity();
	}
	return Props;
}




