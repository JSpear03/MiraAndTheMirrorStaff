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

    // Set the Magic Attack Type Component to Ranged with Damage and Mana Consumption values (placeholder)
    AttackTypeComponent->SetAttackType(TEXT("Ranged"), 4, 5);

    // TODO: Add a way to set the Element for the spell when the component is further developed.
    // This functionality might be best deferred to the player character when casting the spell.

    // Create a larger collision sphere for the ranged ice spell, representing the area it freezes water.
	IceCollision = CreateDefaultSubobject<USphereComponent>(TEXT("IceCollision"));
	IceCollision->InitSphereRadius(20.0f);

	// The ProjectileMovementComponent is responsible for moving the projectile in a straight line at a constant speed.
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent; // CollisionComponent is the root component, so it gets updated.
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->ProjectileGravityScale = 0.f; // It flies perfectly straight
}