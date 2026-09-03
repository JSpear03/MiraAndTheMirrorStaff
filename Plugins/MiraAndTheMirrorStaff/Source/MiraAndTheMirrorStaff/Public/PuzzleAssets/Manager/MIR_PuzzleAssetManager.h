// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"
#include "MIR_PuzzleAssetManager.generated.h"

/**
* The Puzzle-Asset Manager (PAM), in order to handle puzzles, manages two arrays:
*   - `DirectAssetArray`: An array of Direct Puzzle Asset (DPA) instances.
*   - `IndirectAssetArray`: An array of Indirect Puzzle Asset (IPA) instances.
* 
* Upon mutation of one or more assets in the `DirectAssetArray`, the PAM will attempt
* to mutate all assets in the `IndirectAssetArray`, assuming certain conditions are met.
* 
* If all assets in the `DirectAssetArray` are mutated, the PAM will mutate all assets in
* the `IndirectAssetArray` conversely. Thus, the PAM is able to manage puzzle solutions.
*/
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// An array of all DPA instances managed by this actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedAssets|Direct")
		TArray<AMIR_PuzzleAssetDirectBASE*> DirectAssetArray;

	// An array of all IPA instances managed by this actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedAssets|Indirect")
		TArray<AMIR_PuzzleAssetsIndirectBASE*> IndirectAssetArray;

	// The duration of the Auto Timer, which dictates how long the player has to
	// activate/deactivate all DPAs before they are automatically deactivated/activated.
	const float AutoTimerDuration = 0.2f;

	// A handle for the Auto Timer, which will activate or deactivate all linked assets
	// if executed.
	FTimerHandle AutoTimerHandle;

	// Represents whether the Auto Timer should be used.
	// - True  : Use
	// - False : Don't use
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer")
		bool bUseAutoTimer = false;

	// Dictates the following:
	// - True  : All DPAs must be activated to activate the IPAs.
	// - False : All DPAs must be deactivated to deactivate the IPAs.
	bool bIsForwardActivation = true;

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetManager();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ========== LINKED ASSETS | DIRECT ==========

	/**
	* Verifies whether the `DirectAssetRef` parameter, passed by a delegate, is contained
	* in the `DirectAssetArray`.
	* 
	* If so, then the PAM attempts to mutate the IPAs by calling one of the following functions,
	* depending on the value of `bIsForwardActivation`:
	* 
	* - True: `TryIndirectActivation()`
	* 
	* - False: `TryIndirectDeactivation()`
	* 
	* @param DirectAssetRef A reference to an instanced DPA, which broadcasted the delegate.
	*/
	UFUNCTION(BlueprintCallable, Category = "LinkedAssets|Direct")
		void VerifyDirectInArray(AMIR_PuzzleAssetDirectBASE* DirectAssetRef);

	/**
	* Determines whether all DPAs in the `DirectAssetArray` are in a specified state via their
	* `CurrentState` variable.
	*
	* @param InState The state to check for.
	*
	* @returns bool Represents whether all DPAs are in the specified state (True = yes; False = no).
	*/
	UFUNCTION(BlueprintCallable, Category = "LinkedAssets|Direct")
		bool FindAllDirectsInState(EPuzzleState InState);

	// ========== LINKED ASSETS | INDIRECT ==========

	/**
	* If all DPAs are active, it tries to activate the IPAs.
	* Otherwise, it tries to either deactivate the IPAs or set up the Auto Timer,
	* depending on whether the IPAs are already activated or deactivated.
	*/
	void TryIndirectActivation();

	/**
	* If all DPAs are deactive, it tries to deactivate the IPAs.
	* Otherwise, it tries to either activate the IPAs or set up the Auto Timer,
	* depending on whether the IPAs are already activated or deactivated.
	*/
	void TryIndirectDeactivation();
	
	/**
	* Runs a for-each loop on all IPAs, activating them consecutively.
	*/
	void ActivateAllIndirects();

	/**
	* Runs a for-each loop on all IPAs, deactivating them consecutively.
	*/
	void DeactivateAllIndirects();

	/**
	* Returns whether at least one IPA is in the active state.
	* Currently intended as a helper function for `TryIndirectDeactivation()`.
	* 
	* @returns bool
	*/
	bool CheckMinimalActivity();

	/**
	* Returns whether at least one IPA is in the inactive state.
	* Currently intended as a helper function for `TryIndirectActivation()`.
	*
	* @returns bool
	*/
	bool CheckMinimalInactivity();

	// ========== TIMER ==========

	/**
	* If `bUseAutoTimer` is True, the Auto Timer is set using the following:
	* - An event signature depending on the value of `IsForwardActivation?`
	*   - True: `HandleAutoDeactivation()`
	*   - False: `HandleAutoActivation()`
	* - A time dictated by `AutoTimerHandle`.
	* Afterward, sets the `AutoTimerHandle` accordingly.
	*/
	void ExecuteAutoTimer();

	/**
	* Clears and invalidates the `AutoTimerHandle` if it is valid.
	*/
	void ClearAutoTimer();

	/**
	 * Executed if the Auto Timer elapses.
	 * Automatically activates all DPAs and IPAs if all DPAs are NOT in the
	 * Deactivate state.
	 */
	UFUNCTION()
		void HandleAutoActivation();

	/**
	 * Executed if the Auto Timer elapses.
	 * Automatically deactivates all DPAs and IPAs if all DPAs are NOT in the
	 * Activate state.
	 */
	UFUNCTION()
		void HandleAutoDeactivation();

	// ========== MODE ==========

	/**
	* Sets `bIsForwardActivation` depending on the `InitialState` of the `IndirectAsset`.
	* Accordingly, this will dictate how the PAM manages the DPA(s) and IPA(s).
	* Meant to be called exclusively on `BeginPlay()`.
	*/
	void DetermineForwardActivation();
};
