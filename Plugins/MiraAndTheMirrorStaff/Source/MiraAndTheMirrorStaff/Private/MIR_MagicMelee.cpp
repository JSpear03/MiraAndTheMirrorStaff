// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_MagicMelee.h"
#include "MIR_AttackTypeComponent.h"
#include "MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"

AMIR_MagicMelee::AMIR_MagicMelee()
{
    // Set this actor to call Tick() every frame.  
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Set Billboard rotation.
    BillboardComponent->SetRelativeRotation(FRotator3d(InitialRotationX, 0.0, 0.0));

    // Set the init radius to something small for the melee spell as a placeholder
    // For actual implementation, it might be best to use a capsule attached to the end of Mira's staff/dagger
	// and have the collision component follow the staff's tip for melee attacks.
    CollisionComponent->SetCapsuleHalfHeight(80.0f);
    CollisionComponent->SetCapsuleRadius(10.0f);
    CollisionComponent->SetRelativeLocation(FVector(0.0, 0.0, 130.0));

	// Use a larger collision sphere for the melee ice spell, representing the area it freezes water.
    IceCollisionComponent->SetCapsuleHalfHeight(80.0f);
    IceCollisionComponent->SetCapsuleRadius(60.0f);
    IceCollisionComponent->SetRelativeLocation(FVector(50.0, 0.0, 0.0));

    // Add the Melee struct object with the new method. Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->SetCurrentAttackType(EAttackTypes::Melee);
}

void AMIR_MagicMelee::BeginPlay()
{
    Super::BeginPlay();

    // Call the below in Blueprints instead.
    //HandleDespawn(MagicLifespan);
}

void AMIR_MagicMelee::SwingMagic(const float& Alpha)
{
    // The new relative rotation (z-axis) of the Billboard Component.
    double NewRotationZ = FMath::Lerp(InitialRotationZ, FinalRotationZ, Alpha);

    BillboardComponent->SetRelativeRotation(FRotator3d(InitialRotationX, 0.0, NewRotationZ));
}
