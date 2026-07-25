// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_HazardBASE.h"
#include "MIR_HazardSpikes.generated.h"

/**
 * Spikes that float on the surface of water and must be jumped over.
 * They block movement and damage the player on contact, but never
 * teleport the player or delete enemies.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HazardSpikes : public AMIR_HazardBASE
{
	GENERATED_BODY()

public:
	// Sets default values for Spikes' properties.
	AMIR_HazardSpikes();

protected:
	// Called when an actor bumps into the spikes' blocking collision.
	UFUNCTION()
		void OnSpikesHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
