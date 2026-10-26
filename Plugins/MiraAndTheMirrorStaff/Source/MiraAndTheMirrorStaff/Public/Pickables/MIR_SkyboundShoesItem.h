// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_SkyboundShoesItem.generated.h"

/**
 * A pickup that grants the Skybound Shoes (Jump ability) to the player
 * character upon overlap.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_SkyboundShoesItem : public AMIR_PickableBASE
{
	GENERATED_BODY()

public:
	// Sets default values for Skybound Shoes.
	AMIR_SkyboundShoesItem();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Skybound Shoes item.
	*
	* @param OtherActor will be represented by the player.
	*/
	UFUNCTION()
		void OnSkyboundShoesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void DisplaySkyBoundShoes();
		
};
