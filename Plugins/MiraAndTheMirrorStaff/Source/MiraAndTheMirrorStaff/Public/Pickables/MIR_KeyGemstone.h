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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Gemstone pickable.
	*
	* @param OtherActor will be represented by the player.
	*/
	UFUNCTION()
	void OnGemstoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Gemstone")
		void CheckGemColor(AMIR_PlayerChar* CurrentPlayerCharacter);
};
