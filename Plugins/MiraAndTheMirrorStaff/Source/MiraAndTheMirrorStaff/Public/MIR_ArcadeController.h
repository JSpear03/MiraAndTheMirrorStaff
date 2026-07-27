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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02_Interaction",
		meta = (AllowPrivateAccess = "true"))
		float InteractRange = 250.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack")
		bool bCanUseMainAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack")
		int32 MainAttackCooldown = 2;

	// Called to setup input actions. 
	virtual void SetupInputComponent() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to move character.
	void MoveCharacter(const FInputActionValue& ActionValue);

	// Called to attack with Magic or Mirror staff.
	UFUNCTION(BlueprintImplementableEvent)
		void UseMainAttack();

	// Called when player presses the Ranged Copy action. 
	UFUNCTION(BlueprintImplementableEvent)
		void UseMirrorStaff();

	// Called to attack with dagger.
	void UseSubAttack();

	// Discards the current copied magic. 
	UFUNCTION(BlueprintImplementableEvent)

		void DiscardCopiedMagic();

	// Called to interact with an object. 
	void UseInteract();

	// Checks if player has obtained item for jumping.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool IsPlayerJumping();

	// Called to get the player to jump. 
	void StartPlayerJumping();

	// Called after player is done jumping.
	void StopPlayerJumping();

	// Checks if player obtained item for dashing.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool IsPlayerDashing();

	// Called to increase player walk speed.
	void StartDashing();

	// Sets the walk speed back to the initial speed. 
	void StopDashing();

	// Adds the Pause Menu widget to the viewport. 
	UFUNCTION(BlueprintImplementableEvent)
		void PauseMenu();

	// Adds the Map Menu widget to the viewport. 
	UFUNCTION(BlueprintImplementableEvent)
		void OpenMap();

	// Sets Can Use Main Attack to true, so player can use the Main Attack input. 
	UFUNCTION(BlueprintCallable)
		void AllowMainAttack();
};
