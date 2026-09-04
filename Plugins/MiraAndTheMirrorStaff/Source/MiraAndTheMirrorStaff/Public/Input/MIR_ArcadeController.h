// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcadeController.h"
#include "Magic/MIR_MagicBASE.h"
#include "MIR_ArcadeController.generated.h"

/**
 * An Enumeration of all MagicAttacks used in the game.
 * - None: A default value that is not meant to be used in-game.
 * - MainAttack: Specifies either the currently copied magic or the MeleeCopy.
 * - SubAttack: Specifies the Dagger.
 * - RangedCopy: Specifies the RangedCopy.
 */
UENUM(BlueprintType)
enum class EMagicAttacks : uint8
{
	None        UMETA(DisplayName = "None", ToolTip = "A default value that is not meant to be used in-game."),
	MainAttack  UMETA(DisplayName = "MainAttack", ToolTip = "Specifies either the currently copied magic or the MeleeCopy."),
	SubAttack   UMETA(DisplayName = "SubAttack", ToolTip = "Specifies the Dagger."),
	RangedCopy  UMETA(DisplayName = "RangedCopy", ToolTip = "Specifies the RangedCopy."),
};


/**
 * Dictates behaviors for player inputs and controls.
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

	// A reference to the currently summoned magic.
	UPROPERTY(VisibleAnywhere, Category = "Copied Magic")
		AMIR_MagicBASE* SummonedMagicRef;

protected:
	// Represents whether the Main Attack can be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack")
		bool bCanUseMainAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack")
		bool bCanUseSubAttack = true;

	// Window in seconds after using Main Attack before Sub-Attack can be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack",
			  meta = (Units = "Seconds", ClampMin = 0.0, ClampMax = 5.0, UIMin = 0.0, UIMax = 5.0))
		float SubAttackDelayAfterMain = 0.25f;

	// Timer handle for unlocking the sub-attack after a main attack.
	FTimerHandle SubAttackUnlockTimerHandle;

	// Represents the Main Attack's cooldown in terms of seconds.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack",
			  meta = (Units = "Seconds", ClampMin = 0.0, ClampMax = 10.0, UIMin = 0.0, UIMax = 10.0))
		float MainAttackCooldown = 2.0;

	// Called to setup input actions. 
	virtual void SetupInputComponent() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the controller leaves play or is destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Called to move character.
	 *
	 * @param ActionValue The value with which the character will be moved.
	 * 
	 * @returns N/A
	 */
	void MoveCharacter(const FInputActionValue& ActionValue);

	// Input handlers that enforce gating, mana cooldown, and lockout delay.
	void HandleMainAttackInput();
	void HandleRangedCopyInput();
	void HandleSubAttackInput();

	// Unlocks sub-attack after the brief window following a main attack.
	void UnlockSubAttack();

	// Called to attack with Magic or Mirror staff.
	UFUNCTION(BlueprintImplementableEvent)
		void UseMainAttack();

	// Called when player presses the Ranged Copy action. 
	UFUNCTION(BlueprintImplementableEvent)
		void UseMirrorStaff();

	// Called to attack with dagger.
	UFUNCTION(BlueprintImplementableEvent)
		void UseSubAttack();

	// Discards the current copied magic. 
	UFUNCTION(BlueprintImplementableEvent)
		void DiscardCopiedMagic();

	// Called to interact with an object. 
	void UseInteract();

	/**
	 * Checks if player has obtained the Skybound Shoes, the item that allows jumping.
	 * 
	 * @returns bool
	 */
	bool IsPlayerJumping() const;

	// Called to get the player to jump.
	void StartPlayerJumping();

	// Called after player is done jumping.
	void StopPlayerJumping();

	/**
	 * Checks if player has obtained the Spirit Stole, the item that allows dashing.
	 *
	 * @returns bool
	 */
	bool IsPlayerDashing() const;

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

	// Sets `SummonedMagicRef` to a reference to a summoned magic.
	UFUNCTION(BlueprintCallable, Category = "Copied Magic")
		void SetSummonedMagicRef(AMIR_MagicBASE* const& MagicType);

	UFUNCTION(BlueprintCallable, Category = "Copied Magic")
		AMIR_MagicBASE* GetSummonedMagicRef() const;

public:
	// Sets `bCanUseMainAttack` to True, re-enabling the Main Attack input.
	UFUNCTION(BlueprintCallable)
		void AllowMainAttack();
};
