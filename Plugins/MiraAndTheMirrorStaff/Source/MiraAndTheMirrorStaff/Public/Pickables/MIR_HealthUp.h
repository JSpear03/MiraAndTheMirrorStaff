// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickables/MIR_PickableBASE.h"
#include "MIR_HealthUp.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HealthUp : public AMIR_PickableBASE
{
	GENERATED_BODY()

	const int32 HealthBonus = 2;
	
public:	
	// Sets default values for this actor's properties
	AMIR_HealthUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Health Up actor. 
	* 
	* @param OtherActor will be represented by the player. 
	*/
	UFUNCTION()
		void OnHealthOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
