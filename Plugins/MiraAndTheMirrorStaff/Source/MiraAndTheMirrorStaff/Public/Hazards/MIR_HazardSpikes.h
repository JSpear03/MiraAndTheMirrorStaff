// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Hazards/MIR_HazardBASE.h"
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
	/**
	 * Called when an actor overlaps with this actor.
	 *
	 * @param HitComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap.
	 * @param OtherComp The specific primitive that began the overlap.
	 * @param NormalImpulse The total impulse applied along the collision normal
	 * during the physics-simulation step.
	 * @param Hit Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OnSpikesHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
