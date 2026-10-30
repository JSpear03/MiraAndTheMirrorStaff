// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "MIR_OnOffButton.generated.h"

/**
 * A type of DPA that activates and deactivates things when the player steps
 * on and off it, respectively.
 */
UCLASS(Abstract)
class MIRAANDTHEMIRRORSTAFF_API AMIR_OnOffButton : public AMIR_PuzzleAssetDirectBASE
{
	GENERATED_BODY()

	// Whether the button is allowed to reset its state.
	// Basically a one-shot where, if true, it never resets.
	UPROPERTY(EditAnywhere, Category = "Overlap")
		bool bIsNothingFired = false;
	
public:	
	// Sets default values for this actor's properties.
	AMIR_OnOffButton();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when an actor collides with the button.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComponent The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	/**
	 * Called when an actor stops colliding with the button.
	 *
	 * @param OverlappedComponent The component that ended the overlap.
	 * @param OtherActor The actor that ended the overlap (ideally a player character).
	 * @param OtherComponent The specific primitive that ended the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that ended
	 * the overlap.
	 */
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;
};
