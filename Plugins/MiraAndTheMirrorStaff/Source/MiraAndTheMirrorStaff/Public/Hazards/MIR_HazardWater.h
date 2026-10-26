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

	// Extends the base hazard behavior with Ice-element attack detection.
	virtual void OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	// Blueprint hook fired when an Ice-element attack touches the water.
	// The future grid-freeze system will consume this to freeze the touched squares.
	UFUNCTION(BlueprintImplementableEvent, Category = "Hazard|Freeze")
		void OnFreezeHit(FVector HitLocation);
};
