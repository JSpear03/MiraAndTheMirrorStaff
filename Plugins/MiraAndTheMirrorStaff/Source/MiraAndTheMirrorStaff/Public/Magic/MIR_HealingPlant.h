// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "MIR_HealingPlant.generated.h"

/**
 * An unmovable plant with wondrous healing properties.
 * The game will end in success if the player copies it.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HealingPlant : public AActor
{
	GENERATED_BODY()

	// Healing plant collision.
	UPROPERTY(EditAnywhere, Category = "Collision")
		UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere, Category = "Collision")
		UBoxComponent* BoxCollision;

	// Healing plant mesh.
	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AMIR_HealingPlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Healing Plant.
	*
	* @param OtherActor is represented by the magic attack.
	*/
	UFUNCTION()
		void OnHealingPlantOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	/*
	* When something overlaps with the box collision.
	*
	* @param OtherActor is represented by the player.
	*/
	UFUNCTION()
		void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	/*
	* When something stops colliding with the box collision.
	*
	* @param OtherActor is represented by the player.
	*/
	UFUNCTION()
		void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Transitions to the epilogue level containing the Epilogue UI.  
	UFUNCTION(BlueprintImplementableEvent, Category = "Epilouge")
		void OpenEpilogue();

	// Inserts copy message to the viewport.
	UFUNCTION(BlueprintImplementableEvent, Category = "Plant")
		void OpenPlantCopyUI();

	// Removes copy message from the viewport.
	UFUNCTION(BlueprintImplementableEvent, Category = "Plant")
		void ClosePlantCopyUI();

};
