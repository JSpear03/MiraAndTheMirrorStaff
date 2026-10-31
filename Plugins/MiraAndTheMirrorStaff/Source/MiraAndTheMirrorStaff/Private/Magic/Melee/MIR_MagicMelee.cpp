// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Magic/Melee/MIR_MagicMelee.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"

AMIR_MagicMelee::AMIR_MagicMelee()
{
    // Set this actor to call Tick() every frame.  
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create default subobjects.
    SwingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SwingTimeline"));

    // Set Billboard rotation.
    BillboardComponent->SetRelativeRotation(FRotator3d(InitialRotationX, 0.0, 0.0));

    // Set the init radius to something small for the melee spell as a placeholder
    // For actual implementation, it might be best to use a capsule attached to
    // the end of Mira's staff/dagger and have the CollisionComponent follow
    // the staff's tip for melee attacks.
    CollisionComponent->SetCapsuleHalfHeight(80.0f);
    CollisionComponent->SetCapsuleRadius(10.0f);
    CollisionComponent->SetRelativeLocation(FVector(0.0, 0.0, 130.0));

	// Use a larger collision sphere for the melee ice spell, representing
    // the area it freezes water.
    IceCollisionComponent->SetCapsuleHalfHeight(80.0f);
    IceCollisionComponent->SetCapsuleRadius(60.0f);
    IceCollisionComponent->SetRelativeLocation(FVector(50.0, 0.0, 0.0));

    // Add the Melee struct object with the new method.
    // Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->SetCurrentAttackType(EAttackTypes::Melee);

    // Override the Niagara Component's attachment (normally BillboardComponent
    // in the base class).
    NiagaraComponent->SetupAttachment(CollisionComponent);
}

void AMIR_MagicMelee::BeginPlay()
{
    Super::BeginPlay();

    // Set up and play Timeline from the start.
    SetUpTimeline();
    SwingTimeline->PlayFromStart();
}

void AMIR_MagicMelee::SwingUpdate(float Alpha)
{
    SwingMagic(Alpha);
}

void AMIR_MagicMelee::SwingComplete()
{
    HandleDespawn(MagicLifespan);

    // NOTE: Originally, in the Blueprint, code was called to set `bCanUseSubAttack`
    // to true, but its efficacy is unknown. The code would switch on
    // `CurrentAttackSubtype` and, in the Dagger case, would cast to MIR_ArcadeController
    // toggle said variable to true.
}

void AMIR_MagicMelee::SwingMagic(const float& Alpha)
{
    // The new relative rotation (z-axis) of the Billboard Component.
    float NewRotationZ = FMath::Lerp(InitialRotationZ, FinalRotationZ, Alpha);

    BillboardComponent->SetRelativeRotation(FRotator3d(InitialRotationX, 0.0, NewRotationZ));
}

void AMIR_MagicMelee::SetUpTimeline()
{
    // Bind function signatures.
    SwingAlpha.BindUFunction(this, FName("SwingUpdate"));
    SwingCompleteEvent.BindUFunction(this, FName("SwingComplete"));

    // Set up `PanningCurve`.
    SetUpCurve();

    // Link timeline events.
    SwingTimeline->AddInterpFloat(SwingCurve, SwingAlpha);
    SwingTimeline->SetTimelineFinishedFunc(SwingCompleteEvent);
}

void AMIR_MagicMelee::SetUpCurve()
{
    // If a curve is already assigned in the editor, don't continue;
    // the following code sets up a native curve.
    if (SwingCurve) { return; }

    // Set up `PanningCurve`.
    SwingCurve = NewObject<UCurveFloat>();

    // Add the two keys.
    SwingCurve->FloatCurve.AddKey(0.0f, 0.0f);   // Time = 0;    Value = 0
    SwingCurve->FloatCurve.AddKey(0.25f, 1.0f);  // Time = 0.25; Value = 1

    // Set the interpolation mode on said keys.
    /* The current key in `SwingCurve`. */
    for (FRichCurveKey& Key : SwingCurve->FloatCurve.Keys)
    {
        Key.InterpMode = RCIM_Linear;
    }
}
