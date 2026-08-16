// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_MagicBASE.h"
#include "MIR_AttackTypeComponent.h"
#include "MIR_ElementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MIR_ArcadeController.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"

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
}

// Called when the game starts or when spawned
void AMIR_MagicBASE::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMIR_MagicBASE::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMIR_MagicBASE::HandleDespawn(const float TimerDuration)
{
    // Prepare variables for timer.
    FTimerHandle DespawnTimerHandle;   // The handle for the despawn timer.
    const UWorld* World = GetWorld();  // A reference to the World.
    FTimerManager& TimerManager = World->GetTimerManager();  // Timer manager for the despawn timer.

    // Lambda for handling despawn upon elapsion of timer.
    const FTimerDelegate& DespawnTimerDelegate = FTimerDelegate::CreateLambda([this, &TimerManager, &DespawnTimerHandle]
        {
            // Debug message (TODO: Delete once debugging is complete).
            UE_LOG(LogTemp, Warning, TEXT("Lifespan elapsed; destroy magic."));

            TimerManager.ClearTimer(DespawnTimerHandle);  // Clears timer.
            ReenableMainAttack();
            this->Destroy();                              // Destroys actor.
        }
    );

    // Initiate timer.
    TimerManager.SetTimer(DespawnTimerHandle, DespawnTimerDelegate, TimerDuration, false);
}

void AMIR_MagicBASE::InstantHandleDespawn()
{
    ReenableMainAttack();
    this->Destroy();
}

void AMIR_MagicBASE::ReenableMainAttack()
{
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
    }
}
