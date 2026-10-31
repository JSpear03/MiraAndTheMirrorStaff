// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"
#include "Components/BoxComponent.h"
#include "MIR_EnemyManager.generated.h"

/**
* The Enemy Manager, in order to handle enemy-locked puzzles, manages two arrays:
*   - `EnemyArray`: An array of EnemyBASE instances.
*   - `IndirectAssetArray`: An array of Indirect Puzzle Asset (IPA) instances.
*
* Upon mutation of one or more assets in the `EnemyArray`, the manager will attempt
* to mutate all assets in the `IndirectAssetArray`, assuming certain conditions are met.
*
* If all assets in the `EnemyArray` are mutated, the manager will mutate all assets in
* the `IndirectAssetArray` accordingly. Thus, it is able to manage puzzle solutions.
*/
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_EnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// An array of all enemy instances managed by this actor.
	UPROPERTY(EditAnywhere, Category = "LinkedAssets|Enemies")
		TArray<AMIR_EnemyBASE*> EnemyArray;

	// An array of all IPA instances managed by this actor.
	UPROPERTY(EditAnywhere, Category = "LinkedAssets|Indirect Puzzle Assets")
		TArray<AMIR_PuzzleAssetsIndirectBASE*> IndirectAssetArray;

	// Dictates the following:
	// - True  : All enemies must be dead to activate the IPAs.
	// - False : All enemies must be dead to deactivate the IPAs.
	UPROPERTY(EditAnywhere, Category = "Mode")
		bool bIsForwardActivation = true;

	// Overlap component for triggering IPA activation/deactivation upon room entry.
	UPROPERTY(EditAnywhere, Category = "Trigger")
		UBoxComponent* TriggerBox;

	// Sets default values for this actor's properties.
	AMIR_EnemyManager();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when an actor, etc. overlaps with this actor.
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
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

private:
	// Tracks how many enemies are dead for comparison against the length of `EnemyArray`.
	UPROPERTY(VisibleAnywhere, Category = "LinkedAssets|Enemies")
		int32 EnemyDeathCount = 0;

	// Represents whether the TriggerBox should trigger continuously throughout its lifespan.
	bool bDoOnce = true;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// ========== LINKED ASSETS | ENEMIES ==========

	/**
	* Verifies whether the `EnemyRef` parameter, passed by a delegate, is contained
	* in the `EnemyArray`.
	*
	* If so, then the manager attempts to mutate the IPAs by calling one of the following
	* functions, depending on the value of `bIsForwardActivation`:
	*
	* - True: `TryIndirectActivation()`
	*
	* - False: `TryIndirectDeactivation()`
	*
	* @param EnemyRef A reference to an instanced enemy, which broadcasted the delegate.
	*/
	UFUNCTION()
		void VerifyEnemyInArray(AMIR_EnemyBASE* EnemyRef);


	// ========== LINKED ASSETS | INDIRECT ==========

	/**
	 * Activates all IPAs consecutively.
	 */
	void ActivateAllIndirects();

	/**
	 * Deactivates all IPAs consecutively.
	 */
	void DeactivateAllIndirects();

	// Stops current level music and plays boss music. 
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Music")
		void PlayBossMusic();

	// Stops boss music and plays the current level music again.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Music")
		void StopBossMusic();

};
