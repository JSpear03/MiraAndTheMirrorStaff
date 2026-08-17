// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_Combat_Component.generated.h"

//Forward declaring Anim Montage so it exists before it exists
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_Combat_Component : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, Category = "AI|Combat")
	bool bIsDeactivated = false;

	// Tracks whether the active attack sequence is still processing
	UPROPERTY(VisibleAnywhere, Category = "AI|Combat")
	bool bAttackFinished = true;

public:

	UMIR_Combat_Component();

	// BT Task Entry Points (called from C++)
	void ExecuteMeleeAttack();
	void ExecuteMagicAttack();

	// Failsafe state reader for the BT Task Tick loop
	bool IsAttackFinished() const { return bAttackFinished; }

	/* --- DESIGNER GRAPH BLUEPRINT NODES ---
	* 
	*		The Designer will need to connect these nodes
	*		up in the blueprint to connect the combat 
	*		functions to the specific magic or melee type
	* 
	*/

	// The designer overrides this event in the enemy Blueprint to fire their specific melee sequence
	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Combat")
	void OnMeleeAttackTriggered();

	// The designer overrides this event in the enemy Blueprint to trigger their magic component sequence
	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Combat")
	void OnMagicAttackTriggered();

	// The designer calls this node inside their Blueprint (or AnimBP) when their attack sequence is fully done
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void FinishCurrentAttack();

	// Returns true if the owner actor has been knocked out / pooled
	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool IsDeactivated() const { return bIsDeactivated; }

	// Returns true if the current attack animation is complete
	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool IsAttackAnimationFinished() const { return bAttackFinished; }

	// Call this from C++ or Blueprints when the enemy dies/pools
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void SetDeactivated(bool bNewDeactivated) { bIsDeactivated = bNewDeactivated; }

	// Resets all AI tracking flags back to their default alive states
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void ReactivateCombatState();

};