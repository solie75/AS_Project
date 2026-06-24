// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/AS_BaseCharacter.h"
#include "InputActionValue.h"                          
#include "EnhancedInputSubsystems.h"
#include "FAS_PlayerInputState.h"
#include "E_MovementStickMode.h"
#include "AS_Player.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGameplayCameraComponent;

UCLASS()
class AS_PROJECT_API AAS_Player : public AAS_BaseCharacter
{
	GENERATED_BODY()

// Base
public:
	AAS_Player(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

// Input
public:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> StrafeAction;

	UPROPERTY(EditAnywhere, Category="Input")
	FAS_PlayerInputState PlayerInputState;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

private:
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnWalkToggle(const FInputActionValue& Value);
	void OnStrafeToggle(const FInputActionValue& Value);
	void OnSprintStart(const FInputActionValue& Value);
	void OnSprintEnd(const FInputActionValue& Value);
	void OnCrouchToggle(const FInputActionValue& Value);

// Camera
private:
	void SetupCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Legacy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Legacy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> LegacyCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|GamePlay", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;

// PreCMC
protected:
	virtual void UpdateRotation_PreCMC() override;

	virtual void UpdateMovement_PreCMC() override;

	// Movement
	virtual E_Gait GetDesiredGait() override;

	virtual bool CanSprint() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	E_MovementStickMode MovementStickMode;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modement")
	TObjectPtr<UCharacterMovementComponent> CMC;*/
	

	// Interface
public:
	virtual void Set_CharacterInputState_Implementation(const FAS_PlayerInputState InputState) override;

	virtual FAS_CharacterAnimProperties Get_PropertiesForAnimation_Implementation() const override;
};
