// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_MagicBASE.h"
#include "MIR_AttackTypeComponent.h"
#include "MIR_ElementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMIR_MagicBASE::AMIR_MagicBASE()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // The Collision Component and its init values (the root component of the spell actor)
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComponent;

    // The Ice Collision Component and its init values (the root component of the spell actor)
    IceCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("IceCollisionComponent"));
    IceCollisionComponent->InitSphereRadius(15.0f);
    IceCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    IceCollisionComponent->SetupAttachment(RootComponent);

	// The base Magic Attack Type Component
	AttackTypeComponent = CreateDefaultSubobject<UMIR_AttackTypeComponent>(TEXT("AttackTypeComponent"));
    AttackTypeComponent->SetAttackTypeDetails();

	// The base Magic Element Component (Fire, Ice, Lightning, None)
	ElementComponent = CreateDefaultSubobject<UMIR_ElementComponent>(TEXT("ElementComponent"));
    ElementComponent->SetElementDetails();

    // Allows for testing collisions. 
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal MeshComponent"));
    Mesh->SetupAttachment(RootComponent);
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