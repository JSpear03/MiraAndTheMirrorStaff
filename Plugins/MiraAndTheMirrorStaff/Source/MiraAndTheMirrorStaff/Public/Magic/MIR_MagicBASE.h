// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "MIR_MagicBASE.generated.h"

UCLASS(BlueprintType)
class MIRAANDTHEMIRRORSTAFF_API AMIR_MagicBASE : public AActor
{
    GENERATED_BODY()

public:
    // A debug-use mesh. Might be deleted, as each magic will likely use a Niagara
    // system for their visuals.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Mesh")
        USkeletalMeshComponent* Mesh;

    // The component which handles the Niagara system to play.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Niagara")
        UNiagaraComponent* NiagaraComponent = nullptr;

private:
    // Timer handle for the despawn timer.
    FTimerHandle DespawnHandle;

public:
    // Sets default values for this actor's properties
    AMIR_MagicBASE();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called when the component ends.
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // The amount of time until the summoned magic despawns.
    const float MagicLifespan = 10.0f;

    /**
     * @brief Handles the despawn of this actor via timer.
     * 
     * Sets up a timer manager, which executes a timer delegate that will cause the actor
     * to be destroyed upon elapsion of the timer.
     * 
     * @param TimerDuration The amount of time until the actor is destroyed.
     * 
     * @returns N/A
     */
    UFUNCTION(BlueprintCallable, Category = "Magic|Despawn")
        void HandleDespawn(const float TimerDuration = 10.0f);

    /*
     * @brief Instantly handles the despawn of this actor; no timer used.
     * 
     * Upon call, this function re-enables the main attack and destroys the actor.
     * 
     * @returns N/A
     */
    void InstantHandleDespawn();

    // Calls `AllowMainAttack()` on an AMIR_ArcadeController reference if it is valid.
    void ReenableMainAttack();

    /*
    * When something overlaps with the Magic Base.
    *
    * @param OtherActor Represented by either an enemy or Indirect puzzle asset.
    */
    /*UFUNCTION()
    void OnMagicBaseOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);*/

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

	// Creates the Attack Type Component for the spell.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Components")
	    class UMIR_AttackTypeComponent* AttackTypeComponent;

    // Creates the Element Component for the spell.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Components")
	    class UMIR_ElementComponent* ElementComponent;

	// Creates the overridable Collision Component for the spell.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Collision")
        class UCapsuleComponent* CollisionComponent;

    // Creates the overridable Collision Component for the ice freeze hitbox on spells.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Collision")
	    class UCapsuleComponent* IceCollisionComponent;

    // A Billboard Component that acts as a pivot for Melee attacks.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Collision")
        class UBillboardComponent* BillboardComponent;

    // A Scene Component that acts as the root component.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Collision")
        class USceneComponent* SceneComponent;

    // Clears the cooldown timer. Must be called during actor cleanup on `EndPlay()`.
    void ClearDespawnTimer();

    /**
     * @brief Attempts to register a hit on the given target actor.
     *
     * Returns true (and records the actor) the first time it is called for a
     * given target. Returns false on every subsequent call for the same target,
     * preventing a single magic instance from damaging the same enemy more than
     * once while still allowing it to hit multiple different enemies.
     *
     * @param Target The actor to register a hit against.
     *
     * @returns True if this is the first hit on Target; false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Magic|Combat")
    bool TryRegisterHit(AActor* Target);

private:
    // Tracks which actors this magic instance has already damaged so that
    // a single spell cannot multi-hit the same enemy (e.g. from dual-capsule
    // overlaps, AoE expansion, or melee swing re-entry).
    UPROPERTY()
    TSet<TObjectPtr<AActor>> DamagedActors;

    // Guard to ensure ReenableMainAttack only runs once per magic actor
    bool bHasReenabledAttack = false;

};