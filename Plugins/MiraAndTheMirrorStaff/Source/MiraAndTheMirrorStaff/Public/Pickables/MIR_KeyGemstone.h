// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_KeyGemstone.generated.h"

/**
 * A gemstone that serves as a key for the final-boss door.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_KeyGemstone : public AMIR_PickableBASE
{
	GENERATED_BODY()
	
public:
	// Sets default values for the Gemstone properties.
	AMIR_KeyGemstone();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when something overlaps with the Key Gemstone item.
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
	void OnGemstoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

public:
	/**
	 * Validates the Material assigned to the gem so that each unique gem can
	 * be marked accordingly in the player's progress. This event also handles
	 * said marking.
	 * 
	 * @param CurrentPlayerCharacter The player that collected this item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Gemstone")
		void CheckGemColor(AMIR_PlayerChar* CurrentPlayerCharacter);
};
