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

    // Create default subobjects.
    ExpansionTimeline =
        CreateDefaultSubobject<UTimelineComponent>(TEXT("ExpansionTimeline"));

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

    // Set up and play Timeline from the start.
    SetUpTimeline();
    ExpansionTimeline->PlayFromStart();
}

void AMIR_MagicAOE::ExpansionUpdate(float Alpha)
{
    MagicExpansion(Alpha);
}

void AMIR_MagicAOE::ExpansionComplete()
{
    HandleDespawn(MagicLifespan);
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

void AMIR_MagicAOE::SetUpTimeline()
{
    // Bind function signatures.
    ExpansionAlpha.BindUFunction(this, FName("ExpansionUpdate"));
    ExpansionCompleteEvent.BindUFunction(this, FName("ExpansionComplete"));

    // Set up `PanningCurve`.
    SetUpCurve();

    // Link timeline events.
    ExpansionTimeline->AddInterpFloat(ExpansionCurve, ExpansionAlpha);
    ExpansionTimeline->SetTimelineFinishedFunc(ExpansionCompleteEvent);
}

void AMIR_MagicAOE::SetUpCurve()
{
    // If a curve is already assigned in the editor, don't continue;
    // the following code sets up a native curve.
    if (ExpansionCurve) { return; }

    // Set up `PanningCurve`.
    ExpansionCurve = NewObject<UCurveFloat>();

    // Add the two keys.
    ExpansionCurve->FloatCurve.AddKey(0.0f, 0.0f);   // Time = 0;    Value = 0
    ExpansionCurve->FloatCurve.AddKey(0.25f, 1.0f);  // Time = 0.25; Value = 1

    // Set the interpolation mode on said keys.
    /* The current key in `ExpansionCurve`. */
    for (FRichCurveKey& Key : ExpansionCurve->FloatCurve.Keys)
    {
        Key.InterpMode = RCIM_Linear;
    }
}
