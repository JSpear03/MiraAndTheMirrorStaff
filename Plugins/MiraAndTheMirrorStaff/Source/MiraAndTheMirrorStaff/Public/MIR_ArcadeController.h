// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcadeController.h"
#include "MIR_ArcadeController.generated.h"

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_ArcadeController : public AArcadeController
{
	GENERATED_BODY()

	// Gameplay Actions needs to be accessed to the editor, so inputs can bind to the functions. 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* MainAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* RangedCopy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* SubAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* Discard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* Interact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* Dash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions", 
		meta = (AllowPrivateAccess = "true"))
		UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01_Gameplay Actions",
		meta = (AllowPrivateAccess = "true"))
		UInputAction* MapAction;


protected:
	// Called to setup input actions. 
	virtual void SetupInputComponent() override;

	// Called to move character.
	void MoveCharacter(const FInputActionValue& ActionValue);

	// Called to attack with Magic or Mirror staff.
	void UseMainAttack();

	// Called when player presses the Ranged Copy action. 
	void UseMirrorStaff();

	// Called to attack with dagger.
	void UseSubAttack();

	// Discards the current copied magic. 
	void DiscardCopiedMagic();

	// Called to interact with an object. 
	void UseInteract();

	// Called to get the player to jump. 
	void StartPlayerJumping();

	// Called after player is done jumping.
	void StopPlayerJumping();

	// Called to increase player walk speed.
	void StartDashing();

	// Sets the walk speed back to the initial speed. 
	void StopDashing();

	// Adds the Pause Menu widget to the viewport. 
	UFUNCTION(BlueprintImplementableEvent)
	void PauseMenu();

	// Adds the Map Mneu widget to the viewport. 
	UFUNCTION(BlueprintImplementableEvent)
	void OpenMap();
};
