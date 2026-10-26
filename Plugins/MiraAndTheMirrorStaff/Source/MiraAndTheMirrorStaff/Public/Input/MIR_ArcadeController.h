// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "ArcadeController.h"
#include "Magic/MIR_MagicBASE.h"
#include "Enums/EMIR_AttackTypes.h"
#include "MIR_ArcadeController.generated.h"

class AMIR_PlayerChar;
class UArrowComponent;
class USoundBase;

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
		float InteractRange = 90.0f;

	// A reference to the currently summoned magic.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Magic",
		meta = (AllowPrivateAccess = "true"))
		AMIR_MagicBASE* SummonedMagicRef = nullptr;

public:
	// Allows Player's Animation Blueprint to play jump animation. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "04_Jump")
		bool bCanAnimateJump = false;

	// Condition when player can move during a certain animation state.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05_Walk")
		bool bCanMove = true;

	// The current attack type state (None, Melee, Ranged, AoE).
	// Read directly by AB_MIR_MiraAnimations to drive attack transition states.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|State")
		EAttackTypes CurrentAttackState = EAttackTypes::None;

	// The magic slot currently active for the attack sequence.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|State")
		EMagicAttacks CurrentMagicSlot = EMagicAttacks::None;

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

	// Minimum cooldown floor (in seconds) between dagger sub-attacks when holding down the button.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack",
			  meta = (Units = "Seconds", ClampMin = 0.0, ClampMax = 5.0, UIMin = 0.0, UIMax = 5.0))
		float MinSubAttackCooldown = 0.4f;

	// Timestamp of the last sub-attack input.
	float LastSubAttackTime = -100.0f;

	// Minimum cooldown floor (in seconds) between attacks even if the projectile despawns prematurely.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03_Main Attack",
			  meta = (Units = "Seconds", ClampMin = 0.0, ClampMax = 5.0, UIMin = 0.0, UIMax = 5.0))
		float MinMainAttackCooldown = 0.8f;

	// Timestamp of the last main attack input.
	float LastMainAttackTime = -100.0f;

	// Timer handle for delaying AllowMainAttack if minimum cooldown has not yet passed.
	FTimerHandle MainAttackUnlockTimerHandle;

	// Fallback unlock timer in case an animation notify is missed or interrupted.
	FTimerHandle AttackSafetyUnlockTimerHandle;

	// Timer handle for resetting attack animation state back to None when animation finishes.
	FTimerHandle AttackStateResetTimerHandle;

	// Fallback timer for spawning projectile if anim notify is missed/unwired in AnimBP.
	FTimerHandle MagicSpawnFallbackTimerHandle;

	// Timer handle for resetting jump animation flag.
	FTimerHandle JumpAnimationTimerHandle;

	// Guards against double spawning if multiple notifies fire in the same attack animation.
	bool bHasSpawnedMagicThisAttack = false;

	// Sound effects for combat actions.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Audio")
		USoundBase* MeleeAttackSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Audio")
		USoundBase* RangedAttackSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Audio")
		USoundBase* AOEAttackSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Audio")
		USoundBase* DaggerAttackSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Audio")
		USoundBase* MirrorStaffAttackSound = nullptr;

	// Internal helper to re-enable main attack and clear unlock timers.
	void ExecuteAllowMainAttack();

	// Starts a timer based on the assigned attack animation length to reset attack state.
	void StartAttackAnimationTimer(EAttackTypes AttackType, bool bIsSubAttack = false);

	// Called when the attack animation finishes.
	void OnAttackAnimationFinished();

	// Starts a fallback timer to spawn ranged/AoE magic if the notify isn't triggered.
	void StartSpawnFallbackTimer();

	// Fallback spawner for ranged/AoE magic.
	void TriggerSpawnFallback();


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
	 */
	void MoveCharacter(const FInputActionValue& ActionValue);

	// Tracks whether attack input actions are currently held down for hold-to-repeat
	bool bIsMainAttackHeld = false;
	bool bIsRangedCopyHeld = false;
	bool bIsSubAttackHeld = false;

	// Input handlers that enforce gating, mana cooldown, and lockout delay.
	void HandleMainAttackInput();
	void HandleRangedCopyInput();
	void HandleSubAttackInput();

	// Started / Completed input delegates to support hold-to-repeat attack
	void OnMainAttackStarted();
	void OnMainAttackCompleted();
	void OnRangedCopyStarted();
	void OnRangedCopyCompleted();
	void OnSubAttackStarted();
	void OnSubAttackCompleted();

	// Checks if any attack input is held and triggers repeat attack when animation completes
	void CheckAutoRepeatAttack();

	// Unlocks sub-attack after the brief window following a main attack.
	void UnlockSubAttack();

	// Native Attack Actions. Override in Blueprint if needed, or rely on C++ implementation.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Input")
		void UseMainAttack();
	virtual void UseMainAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Input")
		void UseMirrorStaff();
	virtual void UseMirrorStaff_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Input")
		void UseSubAttack();
	virtual void UseSubAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Input")
		void DiscardCopiedMagic();
	virtual void DiscardCopiedMagic_Implementation();

	// Resets stored copied magic and player element back to default (MeleeCopy).
	void ResetCopiedMagic();

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

public:
	// Helper to obtain the controlled AMIR_PlayerChar.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Player")
		AMIR_PlayerChar* GetPlayerCharacter() const;

	// Resolves the magic class for a given slot (Dagger, RangedCopy, or Stored/MeleeCopy).
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Magic")
		TSubclassOf<AMIR_MagicBASE> GetAppropriateMagic(EMagicAttacks AttackSlot) const;

	// Returns the EAttackTypes category (Melee, Ranged, AoE) for a given magic class.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Magic")
		EAttackTypes GetAttackTypeForMagic(TSubclassOf<AMIR_MagicBASE> MagicClass) const;

	// Returns the mana cost for a given magic class.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Magic")
		int32 GetManaCostForMagic(TSubclassOf<AMIR_MagicBASE> MagicClass) const;

	// Validates whether the magic associated with AttackSlot can be cast (valid class, sufficient mana, not on cooldown).
	bool CanCastMagic(EMagicAttacks AttackSlot) const;

	// Checks if the specified attack can currently be performed (cooldown, state, mana, etc.).
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|State")
		bool CanPerformAttack(EMagicAttacks AttackSlot) const;

	// Returns the current attack type name for AB_MIR_MiraAnimations state machine transitions.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|State")
		EAttackTypes GetAttackTypeName() const;

	// Gets the appropriate spawn arrow component on the player character for the attack type.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Magic")
		UArrowComponent* GetCorrectSpawnArrow(EAttackTypes AttackType) const;

	// Spawns the magic associated with the given slot, handling mana deduction and placement.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Magic")
		AMIR_MagicBASE* SpawnMagic(EMagicAttacks AttackSlot);
	virtual AMIR_MagicBASE* SpawnMagic_Implementation(EMagicAttacks AttackSlot);

	// Called from Anim Notify in attack animations to spawn projectile/magic at the proper frame.
	UFUNCTION(BlueprintCallable, Category = "Combat|Notify")
		void SpawnRangedProjectileFromNotify();

	UFUNCTION(BlueprintCallable, Category = "Combat|Notify")
		void SpawnAOEFromNotify();

	UFUNCTION(BlueprintCallable, Category = "Combat|Notify")
		void TriggerSpawnFromNotify();

	// Called from Anim Notify AttackLeft in attack animations to restore movement immediately.
	UFUNCTION(BlueprintCallable, Category = "Combat|Notify")
		void OnAttackLeftFromNotify();

	UFUNCTION(BlueprintCallable, Category = "Combat|Notify")
		void AttackLeftFromNotify();


	// Pushes the attack type to the player's AnimInstance (CurrentAttackType) to drive state machine transitions.
	UFUNCTION(BlueprintCallable, Category = "Combat|State")
		void UpdateAnimInstanceAttackType(EAttackTypes NewAttackType);

	// Resets CurrentAttackState and CurrentMagicSlot back to None.
	UFUNCTION(BlueprintCallable, Category = "Combat|State")
		void ResetAttackState();

	// Sets bCanMove to true or false.
	UFUNCTION(BlueprintCallable, Category = "05_Walk")
		void SetCanMove(bool bInCanMove);

	// Sets `SummonedMagicRef` to a reference to a summoned magic.
	UFUNCTION(BlueprintCallable, Category = "Combat|Magic")
		void SetSummonedMagicRef(AMIR_MagicBASE* const& MagicType);

	UFUNCTION(BlueprintCallable, Category = "Combat|Magic")
		AMIR_MagicBASE* GetSummonedMagicRef() const;

	// Sets `bCanUseMainAttack` to True, re-enabling the Main Attack input.
	UFUNCTION(BlueprintCallable, Category = "03_Main Attack")
		void AllowMainAttack();

	// Sets `bCanUseSubAttack` and `bCanUseMainAttack` to True, re-enabling attacks.
	UFUNCTION(BlueprintCallable, Category = "03_Main Attack")
		void AllowSubAttack();
};
