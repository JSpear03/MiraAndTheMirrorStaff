// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Magic/MIR_HealingPlant.h"
#include "Magic/MIR_MagicBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_HealingPlant::AMIR_HealingPlant()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	RootComponent = CapsuleCollision;
	Mesh->SetupAttachment(RootComponent);
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_HealingPlant::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, 
		&AMIR_HealingPlant::OnHealingPlantOverlap);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this,
		&AMIR_HealingPlant::OnBoxCollisionBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this,
		&AMIR_HealingPlant::OnBoxCollisionEndOverlap);
}

void AMIR_HealingPlant::OnHealingPlantOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// The magic that overlapped with the plant.
	AMIR_MagicBASE* IncomingMagic = Cast<AMIR_MagicBASE>(OtherActor);

	// Checks if incoming magic is valid.
	// Also, attemps to register hit for the plant only at the first time. 
	if (!IncomingMagic || !IncomingMagic->TryRegisterHit(this))
	{
		return;
	}

	if (IncomingMagic->AttackTypeComponent &&
		IncomingMagic->AttackTypeComponent->GetAttackTypeProperty() == EAttackProperties::Copy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("Plant: Do Copy."));

		OpenEpilogue();
	}
}

void AMIR_HealingPlant::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) { return; }

	// Points to the player character that overlapped with the door.
	AMIR_PlayerChar* PlayerChar = Cast<AMIR_PlayerChar>(OtherActor);

	// Only show healing message when player interacts.
	if (PlayerChar)
	{
		OpenPlantCopyUI();
	}
}

void AMIR_HealingPlant::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (!OtherActor) { return; }

	// Points to the player character that overlapped with the door.
	AMIR_PlayerChar* PlayerChar = Cast<AMIR_PlayerChar>(OtherActor);

	// Only show healing message when player interacts.
	if (PlayerChar)
	{
		ClosePlantCopyUI();
	}
}

// Called every frame
void AMIR_HealingPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
