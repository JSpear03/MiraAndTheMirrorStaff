// Fill out your copyright notice in the Description page of Project Settings.


#include "Magic/Ranged/MIR_MagicRanged.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMIR_MagicRanged::AMIR_MagicRanged()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Placeholder value for the main damaging collision of the ranged spell.
    CollisionComponent->SetCapsuleRadius(40.0f);
    CollisionComponent->SetCapsuleHalfHeight(40.0f);

    // Placeholder value for a larger collision sphere for the ranged ice spell, representing the area it freezes water.
    IceCollisionComponent->SetCapsuleHalfHeight(90.0f);
    IceCollisionComponent->SetCapsuleRadius(60.0f);
    IceCollisionComponent->SetRelativeLocation(FVector(0.0, 0.0, -40.0));

    // Add the Ranged struct object with the new method. Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->SetCurrentAttackType(EAttackTypes::Ranged);

	// The ProjectileMovementComponent is responsible for moving the projectile in a straight line at a constant speed.
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SceneComponent; // SceneComponent is the root component, so it gets updated.
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->ProjectileGravityScale = 0.f; // It flies perfectly straight

    // Set lifespan.
    InitialLifeSpan = MagicLifespan;
}

void AMIR_MagicRanged::BeginPlay()
{
    Super::BeginPlay();

    //HandleDespawn(MagicLifespan);
}

void AMIR_MagicRanged::Tick(float DeltaTime)
{
    // Execute only when needed.
    if (!bDoOnce)
    {
        // The current Player Controller at index 0.
        APlayerController* PlayerCont = UGameplayStatics::GetPlayerController(this, 0);

        // Validate Player Controller.
        if (!PlayerCont)
        {
            // Invalid, so exit.
            return;
        }

        // Set up remaining variables for `ProjectWorldToScreen()` below.
        FVector WorldPosition = this->GetActorLocation();  // The world position of this actor.
        FVector2D ScreenPosition;                          // The screen position of this actor.

        // Project the actor's world position to a screen position.
        UGameplayStatics::ProjectWorldToScreen(PlayerCont, WorldPosition, ScreenPosition, false);

        // Get the viewport size for use in the conditional statement below.
        int32 ViewportWidth;   // The viewport's width.
        int32 ViewportHeight;  // The viewport's height.
        PlayerCont->GetViewportSize(ViewportWidth, ViewportHeight);

        // Check whether the actor, per its `ScreenPosition`, has exited the viewport.
        if ((ScreenPosition.X <= 0.0) || (ScreenPosition.X >= ViewportWidth) ||
            (ScreenPosition.Y <= 0.0) || (ScreenPosition.Y >= ViewportHeight))
        {
            // Has exited the viewport, so destroy it.
            InstantHandleDespawn();
            bDoOnce = true;  // Disable execution (as it would repeat otherwise).
        }
    }
}
