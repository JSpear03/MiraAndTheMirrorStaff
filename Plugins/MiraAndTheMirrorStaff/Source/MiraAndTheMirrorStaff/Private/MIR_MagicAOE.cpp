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

    // Use an identically sized collision sphere for the AOE ice spell, representing the area it freezes water.
    IceCollisionComponent->InitSphereRadius(50.0f);

    // Add the AOE struct object with the new method. Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->AttackType.AttackTypeName = "AOE";
    AttackTypeComponent->AttackType.Damage = 4;
    AttackTypeComponent->AttackType.ManaConsumption = 3;
    AttackTypeComponent->SetAttackTypeDetails(EAttackTypes::AoE);
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