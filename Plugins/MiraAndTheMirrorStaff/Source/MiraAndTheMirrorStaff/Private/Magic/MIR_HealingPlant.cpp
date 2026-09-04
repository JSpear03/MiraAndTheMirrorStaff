// Fill out your copyright notice in the Description page of Project Settings.


#include "Magic/MIR_HealingPlant.h"
#include "Magic/MIR_MagicBASE.h"

// Sets default values
AMIR_HealingPlant::AMIR_HealingPlant()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	RootComponent = CapsuleCollision;
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_HealingPlant::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, 
		&AMIR_HealingPlant::OnHealingPlantOverlap);
}

void AMIR_HealingPlant::OnHealingPlantOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	AMIR_MagicBASE* IncomingMagic = Cast<AMIR_MagicBASE>(OtherActor);

	// Checks if incoming magic is valid.
	// Also, attemps to register hit for the plant only at the first time. 
	if (!IncomingMagic || !IncomingMagic->TryRegisterHit(this))
	{
		return;
	}

	if (IncomingMagic->AttackTypeComponent && IncomingMagic->AttackTypeComponent->GetAttackTypeProperty() == EAttackProperties::Copy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("Do Copy"));

		OpenEpilogue();
	}
}

// Called every frame
void AMIR_HealingPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
