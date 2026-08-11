// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_MagicRanged.h"
#include "MIR_AttackTypeComponent.h"
#include "MIR_ElementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMIR_MagicRanged::AMIR_MagicRanged()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Placeholder value for the main damaging collision of the ranged spell.
    CollisionComponent->InitSphereRadius(10.0f);

    // Placeholder value for a larger collision sphere for the ranged ice spell, representing the area it freezes water.
    IceCollisionComponent->InitSphereRadius(20.0f);

    // Add the Ranged struct object with the new method. Uses settled consumption values, placeholder for damage.
	AttackTypeComponent->AttackType.AttackTypeName = "Ranged";
	AttackTypeComponent->AttackType.Damage = 4;
	AttackTypeComponent->AttackType.ManaConsumption = 1;
    AttackTypeComponent->SetAttackTypeDetails(EAttackTypes::Ranged);

	// The ProjectileMovementComponent is responsible for moving the projectile in a straight line at a constant speed.
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent; // CollisionComponent is the root component, so it gets updated.
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->ProjectileGravityScale = 0.f; // It flies perfectly straight
}