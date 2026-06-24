// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_Character_FoleyAudioComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UAS_Character_FoleyAudioComponent::UAS_Character_FoleyAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAS_Character_FoleyAudioComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAS_Character_FoleyAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAS_Character_FoleyAudioComponent::PlayLandFoley(FGameplayTag EventTag, FAS_FoleyEventParams Param)
{
	if (!FoleyAudioBank) return;

	USoundBase* FoleySound  = nullptr;
	if (!FoleyAudioBank->GetSoundFromFoleyEvent(EventTag, FoleySound )) return;

	UGameplayStatics::SpawnSoundAttached(
	FoleySound ,                              // 재생할 사운드 에셋
	this,                               // 부착할 컴포넌트 (FoleyAudioComponent 자신)
	NAME_None,                          // 소켓 이름 (없음)
	FVector::ZeroVector,                // 오프셋 위치 (컴포넌트 위치 그대로)
	EAttachLocation::KeepRelativeOffset,// 부착 방식 (상대 위치 유지)
	false,                              // bStopWhenAttachedToDestroyed (컴포넌트 소멸 시 중단 여부)
	Param.Volume,                       // 볼륨 배수
	Param.Pitch                         // 피치 배수
	);
}
