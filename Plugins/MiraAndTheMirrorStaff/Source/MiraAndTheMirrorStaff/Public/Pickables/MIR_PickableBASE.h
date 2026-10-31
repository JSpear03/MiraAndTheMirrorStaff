// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "MIR_PickableBASE.generated.h"

/**
 * An Actor that grants an effect or item to the player upon overlap.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PickableBASE : public AActor
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties.
	AMIR_PickableBASE();

protected:
	// Collision for any child of PickableBASE.
	UPROPERTY(EditAnywhere, Category = "CollisionComponent")
		USphereComponent* CollisionComponent;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Mesh for any child of PickableBASE. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Mesh;

	// Unique identifier for save tracking. If None, defaults to LevelName_ActorName.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		FName PickupID = NAME_None;

	// Helper to resolve the effective PickupID.
	UFUNCTION(BlueprintPure, Category = "Save")
		FName GetEffectivePickupID() const;

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

};
