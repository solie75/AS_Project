// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_FoleyAudioBank.h"
#include "GameplayTagContainer.h"
#include "Components/AS_FoleyAudioEntry.h"

bool UAS_FoleyAudioBank::GetSoundFromFoleyEvent(FGameplayTag Event, USoundBase*& OutSound)
{
	for (const FAS_FoleyAudioEntry& Entry : Assets)
	{
		if (Entry.Event == Event)
		{
			OutSound = Entry.Sound;
			return true;
		}
	}
	
	return false;
}
