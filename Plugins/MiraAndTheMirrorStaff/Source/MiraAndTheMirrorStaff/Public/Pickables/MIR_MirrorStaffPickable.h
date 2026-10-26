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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Mirror Staff pickable.
	*
	* @param OtherActor will be represented by the player.
	*/
	UFUNCTION()
	void OnMirrorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
