// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_SpiritStoleItem.generated.h"

/**
 * A pickup that grants the Spirit Stole (Dash ability) to the player
 * character upon overlap.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_SpiritStoleItem : public AMIR_PickableBASE
{
	GENERATED_BODY()
	

public:
	// Sets default values for Spirit Stole item.
	AMIR_SpiritStoleItem();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when something overlaps with the Spirit Stole item.
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
		void OnSpiritStoleOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Displays tutorial widget.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void DisplaySpiritStole();
};
