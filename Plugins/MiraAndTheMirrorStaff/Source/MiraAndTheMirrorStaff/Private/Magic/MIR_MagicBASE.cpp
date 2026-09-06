// Fill out your copyright notice in the Description page of Project Settings.


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

    // The Scene Component and its init values (the root component of the spell actor)
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;

    // The Billboard Component and its init values
    BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
    BillboardComponent->SetupAttachment(SceneComponent);

    // The Collision Component and its init values
    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCapsuleHalfHeight(40.0f);
    CollisionComponent->SetCapsuleRadius(40.0f);
    CollisionComponent->SetLineThickness(5.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetupAttachment(BillboardComponent);

    // The Ice-Collision Component and its init values
    IceCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("IceCollisionComponent"));
    IceCollisionComponent->SetCapsuleHalfHeight(60.0f);
    IceCollisionComponent->SetCapsuleRadius(60.0f);
    IceCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    IceCollisionComponent->SetupAttachment(CollisionComponent);

	// The base Magic Attack Type Component
	AttackTypeComponent = CreateDefaultSubobject<UMIR_AttackTypeComponent>(TEXT("AttackTypeComponent"));

	// The base Magic Element Component (Fire, Ice, Lightning, None)
	ElementComponent = CreateDefaultSubobject<UMIR_ElementComponent>(TEXT("ElementComponent"));

    // Allows for testing collisions. 
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal MeshComponent"));
    Mesh->SetupAttachment(BillboardComponent);

    // Set up Niagara Component.
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(BillboardComponent);
    NiagaraComponent->bAutoActivate = false;  // Make invisible by default.
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

    //CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_MagicBASE::OnMagicBaseOverlap);
}

void AMIR_MagicBASE::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearDespawnTimer();  // Clean up despawn timer if the actor is prematurely destroyed.

    Super::EndPlay(EndPlayReason);
}

//void AMIR_MagicBASE::OnMagicBaseOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
//    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    // ========== PREPARE VARIABLES ===============================================================
//    
//    // Represents whether the `OtherActor` is a child of AMIR_EnemyBASE.
//    bool bIsChildOfEnemy = OtherActor->GetClass()->IsChildOf<AMIR_EnemyBASE>();
//
//    //Represents whether the `OtherActor` is a child of AMIR_PuzzleAssetsIndirectBASE.
//    bool bIsChildOfPuzzleAsset = OtherActor->GetClass()->IsChildOf<AMIR_PuzzleAssetsIndirectBASE>();
//
//    // ========== PERFORM VALIDATION ==============================================================
//
//    if (!OtherActor || !bIsChildOfEnemy || !bIsChildOfPuzzleAsset)
//    {
//        return;
//    }
//    else
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Silver, TEXT("Magic Base collision."));
//    }
//
//    // ========== HANDLE COLLISION SCENARIO =======================================================
//
//    if (bIsChildOfEnemy)
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Turquoise,
//                                         TEXT("Magic collided with enemy."));
//    }
//    else if (bIsChildOfPuzzleAsset)
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Emerald,
//                                         TEXT("Magic collided with puzzle asset."));
//    }
//    else
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Orange,
//                                         TEXT("Magic collided with neither enemy nor puzzle asset."));
//    }
//}

// Called every frame
void AMIR_MagicBASE::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMIR_MagicBASE::HandleDespawn(const float TimerDuration)
{
    // Prepare variables for timer.
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();  // Timer manager for the despawn timer.

    // Lambda for handling despawn upon elapsion of timer.
    const FTimerDelegate& DespawnTimerDelegate = FTimerDelegate::CreateLambda([this, &TimerManager]
        {
            // Debug message (TODO: Delete once debugging is complete).
            UE_LOG(LogTemp, Warning, TEXT("Lifespan elapsed; destroy magic."));

            TimerManager.ClearTimer(DespawnHandle);  // Clears timer.
            ReenableMainAttack();
            this->Destroy();                         // Destroys actor.
        }
    );

    // Initiate timer.
    TimerManager.SetTimer(DespawnHandle, DespawnTimerDelegate, TimerDuration, false);
}

void AMIR_MagicBASE::ClearDespawnTimer()
{
    if (GetWorld())  // Validate
    {
        // The TimerManager instance for the current world.
        FTimerManager& TimerManager = GetWorld()->GetTimerManager();

        if (TimerManager.IsTimerActive(DespawnHandle))  // Validate
        {
            // Perform cleanup by clearing timer and invalidating the handle.
            TimerManager.ClearTimer(DespawnHandle);
            DespawnHandle.Invalidate();
        }
    }
}

void AMIR_MagicBASE::InstantHandleDespawn()
{
    ReenableMainAttack();
    SetLifeSpan(0.01f);
    //this->Destroy();
}

void AMIR_MagicBASE::ReenableMainAttack()
{
    // Guard: only execute once per magic instance
    if (bHasReenabledAttack)
    {
        return;
    }
    bHasReenabledAttack = true;

    // Only ignore if this magic was explicitly cast by an enemy.
    // Player-cast magic typically has null instigator/owner when spawned from Blueprint.
    if ((GetInstigator() && GetInstigator()->IsA<AMIR_EnemyBASE>()) ||
        (GetOwner() && GetOwner()->IsA<AMIR_EnemyBASE>()))
    {
        return;
    }

    // The current Player Controller at index 0.
    AMIR_ArcadeController* PlayerCont = Cast<AMIR_ArcadeController>(UGameplayStatics::GetPlayerController(this, 0));

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

bool AMIR_MagicBASE::TryRegisterHit(AActor* Target)
{
    if (!Target || DamagedActors.Contains(Target))
    {
        return false;
    }

    DamagedActors.Add(Target);
    return true;
}
