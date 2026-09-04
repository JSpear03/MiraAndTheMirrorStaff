// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MIR_RespawnPoint.generated.h"

/**
 * A trigger placed at room entrances. When the player overlaps it, it registers
 * itself as the player's LastRespawnPoint, which damaging hazards teleport them to.
 * The actor's own location is used as the spot where the player reappears.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_RespawnPoint : public AActor
{
	GENERATED_BODY()

public:
	// The trigger volume that activates this respawn point.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn")
		class UBoxComponent* TriggerBox;

	// Sets default values for Respawn Point's properties.
	AMIR_RespawnPoint();

protected:
	// Called when an actor enters the trigger volume.
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
};
