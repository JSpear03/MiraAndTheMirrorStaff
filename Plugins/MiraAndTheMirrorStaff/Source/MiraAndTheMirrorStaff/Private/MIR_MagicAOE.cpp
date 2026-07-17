// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_MagicAOE.h"
#include "MIR_AttackTypeComponent.h"
#include "MIR_ElementComponent.h"
#include "Components/SphereComponent.h"

AMIR_MagicAOE::AMIR_MagicAOE()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Placeholder value for the main damaging collision of the AOE spell.
    CollisionComponent->InitSphereRadius(50.0f);

    // Set the Magic Attack Type Component to AOE with Damage and Mana Consumption values (placeholder)
    AttackTypeComponent->SetAttackType(TEXT("AOE"), 4, 5);

    // TODO: Add a way to set the Element for the spell when the component is further developed.
    // This functionality might be best deferred to the player character when casting the spell.
}

// Potential function to allow the AOE spell to expand its radius over time
// If the spell is meant to grow rather than be instant cast.
void AMIR_MagicAOE::SpellExpansion(float InitRadius, float MaxRadius, float LerpTime, float DeltaTime)
{
    // Calculate the new radius based on the elapsed time and the lerp time
    float NewRadius = FMath::Lerp(InitRadius, MaxRadius, DeltaTime / LerpTime);
    // Update the collision component's radius
    CollisionComponent->SetSphereRadius(NewRadius);
}