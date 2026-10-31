// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MIR_RespawnPoint.generated.h"

/**
 * A trigger placed at room entrances. When the player overlaps it, it registers
 * itself as the player's LastRespawnPoint, which damaging hazards teleport them to.
 * The actor's own location is used as the spot where the player reappears.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_RespawnPoint : public AActor
{
	GENERATED_BODY()

public:
	// The trigger volume that activates this respawn point.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn")
		class UBoxComponent* TriggerBox;

	// Sets default values for Respawn Point's properties.
	AMIR_RespawnPoint();

protected:
	/**
	 * Called when an actor overlaps with this actor.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComp The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
};
