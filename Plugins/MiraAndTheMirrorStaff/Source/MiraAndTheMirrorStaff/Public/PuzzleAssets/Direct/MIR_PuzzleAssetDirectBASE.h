// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Player/MIR_PlayerChar.h" 
#include "PuzzleAssets/MIR_PuzzleAssetBASE.h"
#include "Enums/EMIR_PuzzleState.h"
#include "Components/MIR_ElementComponent.h"
#include "MIR_PuzzleAssetDirectBASE.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectActivated,   AMIR_PuzzleAssetDirectBASE*, SelfRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectDeactivated, AMIR_PuzzleAssetDirectBASE*, SelfRef);

/**
 * A Direct Puzzle Asset (DPA) is a puzzle asset that is activated or deactivated by
 * a player character's summoned magic. A DPA may then broadcast a delegate to a Puzzle-
 * Asset Manager (PAM) that is referencing it. From here, the PAM may deactivate or activate
 * its linked Indirect Puzzle Assets (IPAs) accordingly.
 *
 * Thus, DPAs represent the "cause" in the cause–effect relationship between DPAs and IPAs.
 */
UCLASS(Abstract)
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetDirectBASE : public AMIR_PuzzleAssetBASE
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The Element Component, which dictates the actor's interactions with magic.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
		class UMIR_ElementComponent* ElementComponent;

	/**
	 * Activates or deactivates the DPA upon overlap with magic, depending on its
	 * current puzzle state.
	 *
	 * @param OtherActor The actor that overlapped with this actor's `CollisionComponent`.
	 */
	UFUNCTION(BlueprintCallable)
		void HandleOverlap(AActor* OtherActor);

	/**
	 * When the `CapsuleCollision` begins overlapping with something.
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
		virtual void OnCapsuleBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

public:
	// Delegate object to call Activate for Direct.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "State")
		FOnDirectActivated OnDirectActivated;

	// Delegate object to call Deactivate for Direct.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "State")
		FOnDirectDeactivated OnDirectDeactivated;

	// Sets default values for this actor's properties.
	AMIR_PuzzleAssetDirectBASE();

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

};
