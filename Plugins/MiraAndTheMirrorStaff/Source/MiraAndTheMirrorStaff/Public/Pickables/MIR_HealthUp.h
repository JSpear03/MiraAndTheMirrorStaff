// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_HealthUp.generated.h"

/**
 * A pickup that increases a player's health by two points upon overlap.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HealthUp : public AMIR_PickableBASE
{
	GENERATED_BODY()

	// The amount of health to grant the player that collects this item.
	const int32 HealthBonus = 2;
	
public:	
	// Sets default values for this actor's properties.
	AMIR_HealthUp();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when something overlaps with the Health-Up item.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComponent The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OnHealthOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

};
