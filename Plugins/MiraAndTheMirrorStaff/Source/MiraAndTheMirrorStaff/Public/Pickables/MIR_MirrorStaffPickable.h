// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_MirrorStaffPickable.generated.h"

/**
 * A pickup that unlocks the Mirror Staff for the player character upon overlap.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_MirrorStaffPickable : public AMIR_PickableBASE
{
	GENERATED_BODY()
	
public:
	// Constructor to set default values.
	AMIR_MirrorStaffPickable();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when something overlaps with the Mirror Staff item.
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
	void OnMirrorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
