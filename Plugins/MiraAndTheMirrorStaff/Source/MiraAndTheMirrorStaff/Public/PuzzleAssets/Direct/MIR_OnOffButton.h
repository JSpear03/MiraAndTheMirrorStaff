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
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_OnOffButton : public AMIR_PuzzleAssetDirectBASE
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Overlap")
		bool bIsNothingFired = false;
	
public:	
	// Sets default values for this actor's properties
	AMIR_OnOffButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	// Called when player collides with on off button.  
		virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult) override;

	// Called when player stop colliding with the button. 
		void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
