// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hazards/MIR_HazardBASE.h"
#include "MIR_HazardBottomlessPit.generated.h"

/**
 * A bottomless pit where no terrain exists. Damages and respawns the player
 * and deletes enemies. Place the collision box below floor level so actors
 * only overlap it after genuinely falling in.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HazardBottomlessPit : public AMIR_HazardBASE
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties.
	AMIR_HazardBottomlessPit();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when an actor enters the hazard's collision box.
	virtual void OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
};
