// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_PickableBASE.h"
#include "MIR_SpiritStoleItem.generated.h"

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_SpiritStoleItem : public AMIR_PickableBASE
{
	GENERATED_BODY()
	

public:
	// Sets default values for Spirit Stole item. 
	AMIR_SpiritStoleItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Spirit Stole item.
	*
	* @param OtherActor will be represented by the player.
	*/
	UFUNCTION()
	void OnSpiritStoleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
