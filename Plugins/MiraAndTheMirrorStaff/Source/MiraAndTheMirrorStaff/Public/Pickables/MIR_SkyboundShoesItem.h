// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_SkyboundShoesItem.generated.h"

/**
 * 
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
