// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Hazards/MIR_HazardBASE.h"
#include "MIR_HazardWater.generated.h"

/**
 * A body of water. Damages and respawns the player and deletes enemies.
 * Also detects Ice-element attacks touching its surface so that the
 * grid-based freezing system can turn the touched squares into ice paths.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HazardWater : public AMIR_HazardBASE
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties.
	AMIR_HazardWater();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when an actor overlaps with this actor.
	 * Extends the base hazard behavior with Ice-element attack detection.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap.
	 * @param OtherComp The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	virtual void OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	/**
	 * Blueprint hook fired when an Ice-element attack touches the water.
	 * A future grid-freeze system would consume this to freeze the touched squares.
	 * 
	 * @param HitLocation The location of said Ice-element attack.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hazard|Freeze")
		void OnFreezeHit(FVector HitLocation);
};
