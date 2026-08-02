// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_PickableBASE.h"
#include "MIR_ManaUp.generated.h"

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_ManaUp : public AMIR_PickableBASE
{
	GENERATED_BODY()
	
	const int32 ManaBonus = 2;

public:
	// Sets up default values. 
	AMIR_ManaUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Mana Up actor.
	*
	* @param OtherActor will be represented by the player.
	*/
	UFUNCTION()
		void OnManaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
};
