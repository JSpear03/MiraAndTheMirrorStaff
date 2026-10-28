// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_Combat_Component.generated.h"

// Forward declaring Anim Montage so it exists before it exists.
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeleeAttackTriggeredSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMagicAttackTriggeredSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnMagicFromNotifySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFinishedSignature);

/**
 * A Component for managing attack sequences/states and resulting actors summoned.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_Combat_Component : public UActorComponent
{
	GENERATED_BODY()

private:
	// Whether the component is deactivated.
	// Should be true when an enemies dies or is pooled.
	UPROPERTY(VisibleAnywhere, Category = "AI|Combat")
	bool bIsDeactivated = false;

	// Tracks whether the active attack sequence is still processing.
	UPROPERTY(VisibleAnywhere, Category = "AI|Combat")
	bool bAttackFinished = true;

	// Tracks whether magic has already spawned for the current attack.
	UPROPERTY(VisibleAnywhere, Category = "AI|Combat")
	bool bHasSpawnedMagicThisAttack = false;

	// Fallback timer handle if animation notify is missed or unwired.
	FTimerHandle FallbackSpawnTimerHandle;

public:

	UMIR_Combat_Component();

	// BT Task Entry Points (called from C++).
	void ExecuteMeleeAttack();
	void ExecuteMagicAttack();

	// Failsafe state reader for the BT Task Tick loop.
	bool IsAttackFinished() const { return bAttackFinished; }

	// If true, starts a fallback timer when an attack begins so magic spawns
	// even if the Anim Notify is missed/unwired.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	bool bUseFallbackSpawnTimer = false;

	// Delay in seconds before triggering fallback magic spawn if anim notify was not fired.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat",
		meta = (EditCondition = "bUseFallbackSpawnTimer", ClampMin = "0.05", UIMin = "0.05"))
	float FallbackMagicSpawnDelay = 0.5f;

	/* --- DESIGNER GRAPH BLUEPRINT EVENTS ---
	* 
	*		The Designer clicks the component in the Blueprint
	*		and clicks '+' next to these events in the Details panel
	*		to trigger their attack montage / magic sequence.
	* 
	*/

	// Broadcasts in the enemy Blueprint to fire their specific melee sequence.
	UPROPERTY(BlueprintAssignable, Category = "AI|Combat")
	FOnMeleeAttackTriggeredSignature OnMeleeAttackTriggered;

	// Broadcasts in the enemy Blueprint to trigger their magic sequence.
	UPROPERTY(BlueprintAssignable, Category = "AI|Combat")
	FOnMagicAttackTriggeredSignature OnMagicAttackTriggered;

	// Broadcasts when an animation notify triggers the magic/projectile spawn
	// at the exact swing/slash frame.
	UPROPERTY(BlueprintAssignable, Category = "AI|Combat")
	FOnSpawnMagicFromNotifySignature OnSpawnMagicFromNotify;

	// Broadcasts when the attack is completed so Blueprints can reset
	// animation flags (e.g. CanAttack = false).
	UPROPERTY(BlueprintAssignable, Category = "AI|Combat")
	FOnAttackFinishedSignature OnAttackFinished;

	// Called from an Anim Notify (or Anim Blueprint) to trigger magic spawn
	// at the exact frame of the attack.
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void TriggerMagicSpawnFromNotify();

	// The designer calls this node inside their Blueprint (or AnimBP) when
	// their attack sequence is fully done.
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void FinishCurrentAttack();

	// Returns true if the owner actor has been knocked out / pooled.
	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool IsDeactivated() const { return bIsDeactivated; }

	// Returns true if the current attack animation is complete.
	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool IsAttackAnimationFinished() const { return bAttackFinished; }

	// Returns true if magic was already spawned for the current attack.
	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool HasSpawnedMagicThisAttack() const { return bHasSpawnedMagicThisAttack; }

	// Call this from C++ or Blueprints when the enemy dies/pools.
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void SetDeactivated(bool bNewDeactivated);

	// Resets all AI tracking flags back to their default alive states.
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void ReactivateCombatState();

};
