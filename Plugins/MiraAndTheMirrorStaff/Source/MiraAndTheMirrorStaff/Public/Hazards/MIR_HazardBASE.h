// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MIR_HazardBASE.generated.h"

/**
 * Base class for all damaging hazards (Water, Lava, Bottomless Pit, Spikes).
 * When the player enters the collision box, they take damage and are teleported
 * to their last activated respawn point. Enemies that enter are deleted.
 * Subclasses can opt out of the teleport and enemy deletion (see Spikes).
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HazardBASE : public AActor
{
	GENERATED_BODY()

public:
	// The volume that detects actors entering the hazard.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
		UBoxComponent* CollisionBox;

	// Damage dealt to the player on contact, where 1 equals half a heart.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
		int32 Damage = 1;

	// Sets default values for Base Hazard's properties.
	AMIR_HazardBASE();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Whether the player is teleported to their last respawn point on contact.
	bool bTeleportsPlayerToRespawn = true;

	// Whether enemies that enter the hazard are deleted.
	bool bDeletesEnemies = true;

	/**
	 * Called when an actor overlaps with this actor.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap.
	 * @param OtherComp The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		virtual void OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Damages/respawns the player or deletes an enemy based on this hazard's flags.
	 * 
	 * @param OtherActor The actor to evaluate (e.g., a player or enemy).
	 */
	void HandleActorContact(AActor* OtherActor);

	/**
	 * Blueprint hook for contact feedback at the point of impact (splash decals
	 * for Water/Lava, star particles for the Bottomless Pit, etc.).
	 * 
	 * @param The location of the actor that overlapped this actor. Needed for
	 * decal placement.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hazard")
		void OnPlayerContact(FVector ImpactPoint);
};
