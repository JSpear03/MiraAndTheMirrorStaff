// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"
#include "MIR_PuzzleAssetManager.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// An array of all DPA instances managed by this actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedAssets|Direct")
		TArray<AMIR_PuzzleAssetDirectBASE*> DirectAssetArray;

	// The IPA instance managed by this actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedAssets|Indirect")
		AMIR_PuzzleAssetsIndirectBASE* IndirectAsset;

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
		bool bUseAutoTimer = true;

	// Dictates the following:
	// - True  : All DPAs must be activated to activate an IPA.
	// - False : All DPAs must be deactivated to deactivate an IPA.
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
	* If so, then the PAM attempts to mutate the IPA by calling one of the following functions,
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
	* If all DPAs are active, it tries to activate the IPA.
	* Otherwise, it tries to either deactivate the IPA or set up the Auto Timer,
	* depending on whether the IPA is already activated or deactivated.
	*/
	void TryIndirectActivation();

	/*
	* If all DPAs are deactive, it tries to deactivate the IPA.
	* Otherwise, it tries to either activate the IPA or set up the Auto Timer,
	* depending on whether the IPA is already activated or deactivated.
	*/
	void TryIndirectDeactivation();

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
	 * Automatically activates all DPAs and the IPA if all DPAs are NOT in the
	 * Deactivate state.
	 */
	UFUNCTION()
		void HandleAutoActivation();

	/**
	 * Executed if the Auto Timer elapses.
	 * Automatically deactivates all DPAs and the IPA if all DPAs are NOT in the
	 * Activate state.
	 */
	UFUNCTION()
		void HandleAutoDeactivation();

	// ========== MODE ==========

	/**
	* Sets `bIsForwardActivation` depending on the `InitialState` of the `IndirectAsset`.
	* Accordingly, this will dictate how the PAM manages the DPA(s) and IPA.
	* Meant to be called exclusively on `BeginPlay()`.
	*/
	void DetermineForwardActivation();
};
