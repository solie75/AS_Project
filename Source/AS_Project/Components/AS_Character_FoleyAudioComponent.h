// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "AS_FoleyTypes.h"
#include "Asset/AS_FoleyAudioBank.h"
#include "AS_Character_FoleyAudioComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AS_PROJECT_API UAS_Character_FoleyAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAS_Character_FoleyAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public :
	// Land
	UFUNCTION(BlueprintCallable, Category = "Foley")
	void PlayLandFoley(FGameplayTag EventTag, FAS_FoleyEventParams Param);

	// Bank
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foley")
	TObjectPtr<UAS_FoleyAudioBank> FoleyAudioBank;
};
