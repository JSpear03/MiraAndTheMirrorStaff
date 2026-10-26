// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MIR_FinalBossDoor.generated.h"

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

	// Sets default values for this actor's properties
	AMIR_FinalBossDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* When something overlaps with the Final Door.
	*
	* @param OtherActor will be represented by the player.
	*/
	UFUNCTION()
		void OnFinalDoorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Opens the door when player has collided with the actor, and collected all four gems.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Open")
		void OpenDoor();
};
