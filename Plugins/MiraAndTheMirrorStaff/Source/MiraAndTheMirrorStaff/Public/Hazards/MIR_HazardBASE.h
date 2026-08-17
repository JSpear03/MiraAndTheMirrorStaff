// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
		class UBoxComponent* CollisionBox;

	// Damage dealt to the player on contact, where 1 equals half a heart.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard")
		int32 Damage = 1;

	// Sets default values for Base Hazard's properties.
	AMIR_HazardBASE();

protected:
	// Whether the player is teleported to their last respawn point on contact.
	bool bTeleportsPlayerToRespawn = true;

	// Whether enemies that enter the hazard are deleted.
	bool bDeletesEnemies = true;

	// Called when an actor enters the hazard's collision box.
	UFUNCTION()
		virtual void OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	// Damages/respawns the player or deletes an enemy, based on this hazard's flags.
	void HandleActorContact(AActor* OtherActor);

	// Blueprint hook for contact feedback at the point of impact
	// (splash decals for Water/Lava, star particles for the Bottomless Pit, etc.).
	UFUNCTION(BlueprintImplementableEvent, Category = "Hazard")
		void OnPlayerContact(FVector ImpactPoint);
};
