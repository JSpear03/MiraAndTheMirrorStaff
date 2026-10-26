// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/EMIR_PuzzleState.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MIR_PuzzleAssetBASE.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(OnAssetActivated, AMIR_PuzzleAssetBASE, SelfRef);

/**
 * A Puzzle Asset (PA) is an Actor that has an activated and deactivated state,
 * with respective behaviors for each.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetBASE : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMIR_PuzzleAssetBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The puzzle asset's collision component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Collision")
		UCapsuleComponent* CapsuleCollision;

	// The puzzle asset's static mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Mesh")
		UStaticMeshComponent* Mesh;

	// The actor's initial state (activated or deactivated).
	// Determines initialization on BeginPlay.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		EPuzzleState InitialState = EPuzzleState::Deactivate;

	// The actor's current state (activated or deactivated).
	// Initialized to the same value as `InitialState` on BeginPlay.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		EPuzzleState CurrentState = InitialState;

public:
	// Whether magic projectiles can pass through this puzzle asset instead of
	// being destroyed on contact.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
		bool bAllowMagicPassThrough = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Activates the puzzle asset.
	* Meant to be overridden by its children and called with Super.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State",
			  meta = (KeyWords = "Activate Asset"))
		void ActivatePuzzleAsset();
		virtual void ActivatePuzzleAsset_Implementation();

	/**
	* Deactivates the puzzle asset.
	* Meant to be overridden by its children and called with Super.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State",
			  meta = (KeyWords = "Deactivate Asset"))
		void DeactivatePuzzleAsset();
		virtual void DeactivatePuzzleAsset_Implementation();

	/**
	* Gets the `InitialState` member variable.
	* 
	* @returns The PA's initial state.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State",
			  meta = (KeyWords = "Get Initial"))
		const EPuzzleState GetInitialState() const { return InitialState; };

	/**
	* Gets the `CurrentState` member variable.
	* 
	* @returns The PA's current state.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State",
			  meta = (KeyWords = "Get Current"))
		EPuzzleState GetCurrentState() const { return CurrentState; };

	/**
	* Sets the `CurrentState` member variable.
	* 
	* @param NewState The new state of this puzzle asset.
	*/
	UFUNCTION(BlueprintCallable, Category = "State", meta = (KeyWords = "Set Current"))
		void SetCurrentState(EPuzzleState NewState) { CurrentState = NewState; };

};
