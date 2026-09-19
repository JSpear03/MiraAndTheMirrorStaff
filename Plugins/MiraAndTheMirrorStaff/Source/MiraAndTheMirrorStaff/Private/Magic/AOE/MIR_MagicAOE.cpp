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

    // Use an identically sized collision sphere for the AOE ice spell, 
    // representing the area it freezes water.
    IceCollisionComponent->SetCapsuleRadius(InitialSize);
    IceCollisionComponent->SetCapsuleHalfHeight(InitialSize);

    // Add the AOE struct object with the new method. 
    // Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->SetCurrentAttackType(EAttackTypes::AoE);
}

void AMIR_MagicAOE::BeginPlay()
{
    Super::BeginPlay();
}

void AMIR_MagicAOE::MagicExpansion(const float& Alpha)
{
    // The new half-height and radius of the collision components.
    float NewSize = FMath::Lerp(InitialSize, FinalSize, Alpha);

    // Assign the new half-height and radius values to each collision component.
    // CurrentCapsule: The current collision component to modify.
    for (UCapsuleComponent* CurrentCapsule : TCapsuleArray)  
    {
        CurrentCapsule->SetCapsuleHalfHeight(NewSize);
        CurrentCapsule->SetCapsuleRadius(NewSize);
    }

    // Update Niagara System.
    NewSize /= 50.0f;  // (300.0 / 50.0) = 6.0
    NiagaraComponent->SetVariableVec3(TEXT("BodyScale"), FVector(1.0f, 1.0f, 1.0f) * NewSize);
    NiagaraComponent->SetVariableFloat(TEXT("AuraScale"), NewSize);
}
