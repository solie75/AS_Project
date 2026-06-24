// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AS_PreCMCTickComponent.h"
#include "E_Gait.h"
#include "Curves/CurveFloat.h"
#include "Character/Interface/AS_CharacterInterface.h"
#include "Components/AS_Character_FoleyAudioComponent.h"
#include "AS_BaseCharacter.generated.h"


class UAS_CustomCharacterMovementComponent;

UCLASS()
class AS_PROJECT_API AAS_BaseCharacter : public ACharacter , public IAS_CharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAS_BaseCharacter(const FObjectInitializer& ObjectInitializer);

	//AAS_BaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Update
	UFUNCTION()
	void PreCMCTick();

	UPROPERTY(VisibleAnywhere, Category = "Tick")
	TObjectPtr<UAS_PreCMCTickComponent> PreCMCTickComponent;
	
protected:
	UFUNCTION()
	virtual void UpdateRotation_PreCMC();
	
	UFUNCTION()
	virtual void UpdateMovement_PreCMC();

// Movement
public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	E_Gait CurGait;

	UFUNCTION()
	virtual E_Gait GetDesiredGait();

	UFUNCTION()
	virtual bool CanSprint();

	UFUNCTION()
	virtual float CalculateMaxAcceleration();

	UFUNCTION()
	virtual float CalculateMaxSpeed();

	UFUNCTION()
	float CalculateSpeedByDirection(const FVector& Speeds);

	UFUNCTION()
	virtual float CalculateBrakingDeceleration();

	UFUNCTION()
	virtual float CalculateMaxCrouchSpeed();

	UFUNCTION()
	virtual float CalculateGroundFriction();

	UFUNCTION()
	float CalculateSprintMappedValue(float FixedValue, float InRangeA, float InRangeB, float OutRangeA, float OutRangeB);

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Land")
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Land")
	void BP_OnLanded(FAS_FoleyEventParams Params);

private:
	UPROPERTY()
	TObjectPtr<UAS_CustomCharacterMovementComponent> CustomCMC;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TObjectPtr<UCurveFloat> StrafeSpeedMapCurve;

	// X = Forward Speed, Y = Strafe Speed, Z = Backwards Speed
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector WalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector RunSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector SprintSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector CrouchSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bJustLanded = false;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector LandVelocity = FVector::ZeroVector;;

	FTimerHandle LandedTimer;

public:
	UFUNCTION(BlueprintCallable)
	bool Get_bJustLanded() const;

	UFUNCTION(BlueprintCallable)
	FVector Get_LandVelocity() const;

	// Audio
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAS_Character_FoleyAudioComponent> FoleyAudioComponent;

	// CMC
public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE UAS_CustomCharacterMovementComponent* GetCustomCMC() const { return CustomCMC; }
	
	
protected:
	UFUNCTION()
	void SetGait(E_Gait NewGait);
	
	UPROPERTY()
	float AnalogWalkRunThreshold;

	// Interface
public:
	virtual void Set_CharacterInputState_Implementation(const FAS_PlayerInputState InputState) override;
	
	virtual FAS_CharacterAnimProperties Get_PropertiesForAnimation_Implementation() const override;
};
