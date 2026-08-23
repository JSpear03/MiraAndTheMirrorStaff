// Fill out your copyright notice in the Description page of Project Settings.


#include "Magic/AOE/MIR_MagicAOE.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"

AMIR_MagicAOE::AMIR_MagicAOE()
{
    // Set this actor to call Tick() every frame.  
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Placeholder value for the main damaging collision of the AOE spell.
    CollisionComponent->SetCapsuleRadius(InitialSize);
    CollisionComponent->SetCapsuleHalfHeight(InitialSize);

    // Use an identically sized collision sphere for the AOE ice spell, representing the area it freezes water.
    IceCollisionComponent->SetCapsuleRadius(InitialSize);
    IceCollisionComponent->SetCapsuleHalfHeight(InitialSize);

    // Add the AOE struct object with the new method. Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->SetCurrentAttackType(EAttackTypes::AoE);
}

void AMIR_MagicAOE::BeginPlay()
{
    Super::BeginPlay();

    // The below should be called in the Blueprint child class instead.
    //HandleDespawn(MagicLifespan);
}

void AMIR_MagicAOE::MagicExpansion(const float& Alpha)
{
    // The new half-height and radius of the collision components.
    float NewSize = FMath::Lerp(InitialSize, FinalSize, Alpha);

    // Assign the new half-height and radius values to each collision component.
    for (UCapsuleComponent* CurrentCapsule : CapsuleArray)  // The current collision component to modify.
    {
        CurrentCapsule->SetCapsuleHalfHeight(NewSize);
        CurrentCapsule->SetCapsuleRadius(NewSize);
    }
}
