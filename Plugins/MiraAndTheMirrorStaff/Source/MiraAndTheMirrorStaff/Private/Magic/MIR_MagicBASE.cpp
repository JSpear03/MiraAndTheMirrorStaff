// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Magic/MIR_MagicBASE.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Input/MIR_ArcadeController.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"

// Sets default values
AMIR_MagicBASE::AMIR_MagicBASE()
{
    // Set this actor to call Tick() every frame.  
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // The Scene Component and its init values (the root component of the spell actor).
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;

    // The Billboard Component and its init values
    BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
    BillboardComponent->SetupAttachment(SceneComponent);

    SetupCollisions();

	// The base Magic Attack Type Component
	AttackTypeComponent = 
        CreateDefaultSubobject<UMIR_AttackTypeComponent>(TEXT("AttackTypeComponent"));

	// The base Magic Element Component (Fire, Ice, Lightning, None)
	ElementComponent = CreateDefaultSubobject<UMIR_ElementComponent>(TEXT("ElementComponent"));

    // Set up Niagara Component.
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(BillboardComponent);
    NiagaraComponent->bAutoActivate = false;  // Make invisible by default.
}

void AMIR_MagicBASE::SetupCollisions()
{
    // The Ice-Collision Component and its init values
    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCapsuleHalfHeight(40.0f);
    CollisionComponent->SetCapsuleRadius(40.0f);
    CollisionComponent->SetLineThickness(5.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetupAttachment(BillboardComponent);

    // The Ice-Collision Component and its init values.
    IceCollisionComponent = 
        CreateDefaultSubobject<UCapsuleComponent>(TEXT("IceCollisionComponent"));
    IceCollisionComponent->SetCapsuleHalfHeight(60.0f);
    IceCollisionComponent->SetCapsuleRadius(60.0f);
    IceCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    IceCollisionComponent->SetupAttachment(CollisionComponent);
}

// Called when the game starts or when spawned
void AMIR_MagicBASE::BeginPlay()
{
    Super::BeginPlay();

    // Activate Niagara system if valid.
    if (NiagaraComponent->GetAsset())
    {
        // Set colors.
        NiagaraComponent->SetVariableLinearColor(TEXT("LightColor"), ElementComponent->GetMainColor());
        NiagaraComponent->SetVariableLinearColor(TEXT("DarkColor"), ElementComponent->GetSubColor());

        // Activate system (e.g., makes it visible).
        NiagaraComponent->Activate(true);
    }
}

void AMIR_MagicBASE::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearDespawnTimer();  // Clean up despawn timer if the actor is prematurely destroyed.

    Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMIR_MagicBASE::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMIR_MagicBASE::HandleDespawn(const float TimerDuration)
{
    // Prepare variables for timer.

    FTimerManager& FTimer = GetWorld()->GetTimerManager();  // Timer manager for the despawn timer.

    // Lambda for handling despawn upon elapsion of timer.
    const FTimerDelegate& FDespawnTimerDelegate = FTimerDelegate::CreateLambda([this, &FTimer]
        {
            UE_LOG(LogTemp, Warning, TEXT("Lifespan elapsed; destroy magic."));

            FTimer.ClearTimer(FDespawnHandle);  // Clears timer.
            ReenableMainAttack();
            this->Destroy();                    // Destroys actor.
        }
    );

    // Initiate timer.
    FTimer.SetTimer(FDespawnHandle, FDespawnTimerDelegate, TimerDuration, false);
}

void AMIR_MagicBASE::ClearDespawnTimer()
{
    if (!GetWorld())  // Validate
    {
        return;
    }

    // The FTimer instance for the current world.
    FTimerManager& FTimerManager = GetWorld()->GetTimerManager();

    if (FTimerManager.IsTimerActive(FDespawnHandle))  // Validate
    {
        // Perform cleanup by clearing timer and invalidating the handle.
        FTimerManager.ClearTimer(FDespawnHandle);
        FDespawnHandle.Invalidate();
    }
}

void AMIR_MagicBASE::InstantHandleDespawn()
{
    ReenableMainAttack();
    SetLifeSpan(0.01f);
}

void AMIR_MagicBASE::ReenableMainAttack()
{
    bool bHasFoundEnemyInstigator = false, bHasFoundEnemyOwner = false;

    // Guard: only execute once per magic instance
    if (bHasReenabledAttack)
    {
        return;
    }

    bHasReenabledAttack = true;
    bHasFoundEnemyInstigator = GetInstigator() && GetInstigator()->IsA<AMIR_EnemyBASE>();
    bHasFoundEnemyOwner = GetOwner() && GetOwner()->IsA<AMIR_EnemyBASE>();

    // Only ignore if this magic was explicitly cast by an enemy.
    // Player-cast magic typically has null instigator/owner when spawned from Blueprint.
    if (bHasFoundEnemyInstigator || bHasFoundEnemyOwner)
    {
        return;
    }

    // The current Player Controller at index 0.
    AMIR_ArcadeController* PlayerCont = 
        Cast<AMIR_ArcadeController>(UGameplayStatics::GetPlayerController(this, 0));

    // Validate Player Controller.
    if (!PlayerCont)
    {
        // Invalid, so exit.
        return;
    }
    else
    {
        PlayerCont->AllowMainAttack();  // Allows main attack again.
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Main Attack ALLOWED!"));
    }
}

bool AMIR_MagicBASE::TryRegisterHit(AActor* ATarget)
{
    if (!ATarget || TDamagedActors.Contains(ATarget))
    {
        return false;
    }

    TDamagedActors.Add(ATarget);
    return true;
}
