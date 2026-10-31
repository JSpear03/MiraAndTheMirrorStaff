// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "MIR_SavePlatform.generated.h"

/**
 * A platform that opens the Save Screen upon overlap with a player.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_SavePlatform : public AActor
{
	GENERATED_BODY()

	// Collision for the Save Platform.
	UPROPERTY(EditAnywhere, Category = "Collision")
		UBoxComponent* BoxCollision;

	// Mesh for Save Platform.
	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* PlatformMesh;
	
public:	
	// Sets default values for this actor's properties.
	AMIR_SavePlatform();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * @brief Called upon an actor overlapping with the Save Platform.
	 * 
	 * Validates A) the `OtherActor` and the currently possessed player character,
	 * then B) whether said actor is a child of `AMIR_PlayerChar`.
	 * 
	 * If both succeed, then `CallSaveRoomMenu()`--which creates that Widget--is called.
	 * 
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComp The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComp` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OnSaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	/**
	 * Adds the Save-Room UI to the viewport.
	 * 
	 * @param PlayerRef The player that will own the UI.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
		void CallSaveRoomMenu(AMIR_PlayerChar* PlayerRef);
};
