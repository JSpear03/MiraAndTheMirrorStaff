// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Characters/MIR_CharacterBASE_M.h"
#include "Interfaces/IMIR_Copiable.h"
#include "Components/MIR_CopiableComponent.h"
#include "MIR_EnemyBASE.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, AMIR_EnemyBASE*, SelfRef);

/**
 * Base class for all enemies in the game. 
 * Includes logic such as spawning dust, as well as checking if enemy's magic can be copied.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_EnemyBASE : public AMIR_CharacterBASE_M, public IIMIR_Copiable
{
	GENERATED_BODY()

	// Prevents tick from repeating broadcast calls. 
	UPROPERTY()
		bool bCanRunBroadcast = true;
	
public:
	// Sets default value damage when player contacts enemy. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Contact Damage")
		int32 ContactDamage = 1;

	// Implements copy ability. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copy")
		UMIR_CopiableComponent* CopiableComponent;

	// Handles AI combat attack states and trigger events.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class UMIR_Combat_Component* CombatComponent;

	// Delegate to call function when enemy's health is 0.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "State")
		FOnEnemyDied OnEnemyDied;

	// Whether this enemy is a boss variant that should stay defeated permanently once killed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
		bool bIsBossVariant = false;

	// Optional unique identifier for this boss. Defaults to LevelName_ClassName if empty.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss",
			  meta = (EditCondition = "bIsBossVariant"))
		FName BossID;

	// Returns the effective unique ID used for boss defeat save persistence.
	UFUNCTION(BlueprintPure, Category = "Boss")
		FName GetEffectiveBossID() const;

	// Sets default values for Base Enemy.
	AMIR_EnemyBASE();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Called when an actor overlaps with this actor.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap.
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
	/*
	* Sets attack element to flame, ice, or thunder.
	*
	* Incoming magic is used to get the string of the class name.
	*/
	EElements GetEnemyAttackElement(AMIR_MagicBASE* IncomingMagic);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Interface entry point. Any system that wants this enemy's magic (the Mirror Staff's
	 * ranged copy, for example) calls IIMIR_Copiable::Execute_CopyMagic on the enemy Actor,
	 * and the enemy hands the work to its Copiable Component.
	 * 
	 * @param CurrentPlayer Points to a player character.
	 * @param MagicToCopy Magic that the player character should copy.
	 */
	virtual void CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer,
		TSubclassOf<class AMIR_MagicBASE> MagicToCopy) override;

	/**
	 * Called when enemy delgate is broadcasted.
	 * 
	 * @param SelfRef An optional reference to self.
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void EnemyDied(AMIR_EnemyBASE* SelfRef);

	// Change the material to a certain color, after dust is setup. 
	UFUNCTION(BlueprintImplementableEvent)
		void SetNewMaterial();

	// Called by animation notifies to spawn magic at the exact attack frame.
	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual void TriggerMagicSpawnFromNotify();

	// Called by animation notifies to finish attack and return AI to idle/movement.
	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual void TriggerFinishAttackFromNotify();

	// Blueprint event to implement specific enemy magic spawn logic from animation notifies
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
		void OnAnimNotifySpawnMagic();

	// Blueprint event called when the attack completes (use to reset CanAttack = false)
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
		void OnCombatAttackFinished();

private:
	/**
	 * Checks if the enemy's health has reached 0.
	 * If so, it executes logic related to disabling combat and marking bosses
	 * (where applicable) as defeated.
	 */
	void CheckHealth();

};
