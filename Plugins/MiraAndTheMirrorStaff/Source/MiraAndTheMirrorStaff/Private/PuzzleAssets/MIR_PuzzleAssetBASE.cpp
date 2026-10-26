// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "PuzzleAssets/MIR_PuzzleAssetBASE.h"

// Sets default values
AMIR_PuzzleAssetBASE::AMIR_PuzzleAssetBASE()
{
 	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create objects.
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Set up heirarchy.
	RootComponent = CapsuleCollision;
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetBASE::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the puzzle asset's state.
	switch (InitialState)
	{
	case EPuzzleState::Activate:
		ActivatePuzzleAsset();
		break;
	case EPuzzleState::Deactivate:
		DeactivatePuzzleAsset();
		break;
	}
	
}

// Called every frame
void AMIR_PuzzleAssetBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMIR_PuzzleAssetBASE::ActivatePuzzleAsset_Implementation()
{
	SetCurrentState(EPuzzleState::Activate);
}

void AMIR_PuzzleAssetBASE::DeactivatePuzzleAsset_Implementation()
{
	SetCurrentState(EPuzzleState::Deactivate);
}

