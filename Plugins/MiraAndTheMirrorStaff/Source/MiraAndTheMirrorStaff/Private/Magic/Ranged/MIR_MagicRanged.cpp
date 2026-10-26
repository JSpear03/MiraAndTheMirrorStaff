// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Magic/Ranged/MIR_MagicRanged.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Hazards/MIR_HazardBASE.h"
#include "PuzzleAssets/MIR_PuzzleAssetBASE.h"

AMIR_MagicRanged::AMIR_MagicRanged()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Main damaging collision of the ranged spell
    CollisionComponent->SetCapsuleRadius(40.0f);
    CollisionComponent->SetCapsuleHalfHeight(40.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    // Placeholder value for a larger collision sphere for the ranged ice spell, representing the area it freezes water.
    IceCollisionComponent->SetCapsuleHalfHeight(90.0f);
    IceCollisionComponent->SetCapsuleRadius(60.0f);
    IceCollisionComponent->SetRelativeLocation(FVector(0.0, 0.0, -40.0));

    // Add the Ranged struct object with the new method. Uses settled consumption values, placeholder for damage.
    AttackTypeComponent->SetCurrentAttackType(EAttackTypes::Ranged);

	// The ProjectileMovementComponent is responsible for moving the projectile in a straight line at a constant speed.
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SceneComponent;
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->ProjectileGravityScale = 0.f; // It flies perfectly straight

    // Set lifespan.
    InitialLifeSpan = MagicLifespan;
}

void AMIR_MagicRanged::BeginPlay()
{
    Super::BeginPlay();

    if (CollisionComponent)
    {
        CollisionComponent->OnComponentHit.AddDynamic(this, &AMIR_MagicRanged::OnProjectileHit);
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_MagicRanged::OnProjectileOverlap);
    }
}

void AMIR_MagicRanged::Tick(float DeltaTime)
{
    // Execute the following code below the condition, only when needed.
    if (bDoOnce)
    {
        return;
    }

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

void AMIR_MagicRanged::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Ignore hits with self, owner, or instigator
    if (OtherActor && (OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()))
    {
        return;
    }

    // Ignore hazards (water hazards, frozen ice surfaces, lava, spikes, etc.)
    if (OtherActor && (OtherActor->IsA<AMIR_HazardBASE>() || OtherActor->ActorHasTag(TEXT("Hazard"))))
    {
        return;
    }

    // Ignore puzzle assets configured to let magic pass through (torches, orbs, etc.)
    if (AMIR_PuzzleAssetBASE* PuzzleAsset = Cast<AMIR_PuzzleAssetBASE>(OtherActor))
    {
        if (PuzzleAsset->bAllowMagicPassThrough)
        {
            return;
        }
    }

    if (OtherActor && OtherActor->ActorHasTag(TEXT("PassThrough")))
    {
        return;
    }

    // Hit a blocking surface (wall, floor, obstacle) - despawn projectile immediately
    InstantHandleDespawn();
}

void AMIR_MagicRanged::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Only the main damaging capsule collides with walls (do not despawn from the ice-freezing radius)
    if (OverlappedComponent != CollisionComponent)
    {
        return;
    }

    // Ignore overlaps with self, owner, or instigator
    if (OtherActor && (OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator()))
    {
        return;
    }

    // Ignore characters (damage to characters is handled by the character's own overlap)
    if (OtherActor && OtherActor->IsA<ACharacter>())
    {
        return;
    }

    // Ignore hazards (water hazards, frozen ice surfaces, lava, etc.)
    if (OtherActor && (OtherActor->IsA<AMIR_HazardBASE>() || OtherActor->ActorHasTag(TEXT("Hazard"))))
    {
        return;
    }

    // Ignore puzzle assets configured to let magic pass through (torches, orbs, etc.)
    if (AMIR_PuzzleAssetBASE* PuzzleAsset = Cast<AMIR_PuzzleAssetBASE>(OtherActor))
    {
        if (PuzzleAsset->bAllowMagicPassThrough)
        {
            return;
        }
    }

    if (OtherActor && OtherActor->ActorHasTag(TEXT("PassThrough")))
    {
        return;
    }

    // Ignore non-solid volumes (camera triggers, level stream volumes, etc.)
    // Only solid physical barriers (walls, pillars, solid doors) block Pawn movement.
    if (!OtherComp || OtherComp->GetCollisionResponseToChannel(ECC_Pawn) != ECR_Block)
    {
        return;
    }

    // Hit a solid physical wall or obstacle — despawn projectile immediately
    InstantHandleDespawn();
}

