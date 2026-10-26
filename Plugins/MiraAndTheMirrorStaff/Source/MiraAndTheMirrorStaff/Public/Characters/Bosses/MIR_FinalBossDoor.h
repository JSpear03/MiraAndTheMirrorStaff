// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MIR_FinalBossDoor.generated.h"

/**
 * A door that blocks off the final-boss room.
 * It opens only when the player collides with it while
 * having all four Key Gemstones.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_FinalBossDoor : public AActor
{
	GENERATED_BODY()

public:	
	// Collision for the Final Boss door.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
		UBoxComponent* BoxCollision;
	
	// Mesh to represent the door actor.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* DoorMesh;

	// Sets default values for this actor's properties.
	AMIR_FinalBossDoor();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when an actor overlaps with the Final Door.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComponent The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OnFinalDoorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	/**
	 * Opens the door when player, with all four gems, has collided with the actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Open")
		void OpenDoor();
};
