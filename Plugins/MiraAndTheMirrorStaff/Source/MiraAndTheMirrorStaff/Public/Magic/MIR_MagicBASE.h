// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MIR_MagicBASE.generated.h"

UCLASS(BlueprintType)
class MIRAANDTHEMIRRORSTAFF_API AMIR_MagicBASE : public AActor
{
    GENERATED_BODY()

    // A debug-use mesh. Might be deleted, as each magic will likely use a Niagara
    // system for their visuals.
    UPROPERTY(EditAnywhere, Category = "Magic|Mesh")
        USkeletalMeshComponent* Mesh;

public:
    // Sets default values for this actor's properties
    AMIR_MagicBASE();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

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

};