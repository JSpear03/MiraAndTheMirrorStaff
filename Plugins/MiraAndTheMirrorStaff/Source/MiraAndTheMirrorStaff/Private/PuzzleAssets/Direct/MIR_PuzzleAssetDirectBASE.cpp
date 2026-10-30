// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_PuzzleAssetDirectBASE::AMIR_PuzzleAssetDirectBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up Element Component.
	ElementComponent = CreateDefaultSubobject<UMIR_ElementComponent>(TEXT("ElementComponent"));

	// Direct puzzle assets (torches, orbs, etc.) allow magic attacks to pass through by default.
	bAllowMagicPassThrough = true;
}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetDirectBASE::BeginPlay()
{
	Super::BeginPlay();

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AMIR_PuzzleAssetDirectBASE::OnCapsuleBeginOverlap);
}

// Called every frame
void AMIR_PuzzleAssetDirectBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMIR_PuzzleAssetDirectBASE::HandleOverlap(AActor* OtherActor)
{
	if (!OtherActor) { return; }

	// The magic that overlapped this actor.
	AMIR_MagicBASE* OverlappingMagic = Cast<AMIR_MagicBASE>(OtherActor);

	if (!OverlappingMagic) { return; }

	// Determine whether to activate or deactivate the DPA.
	switch (CurrentState)
	{
	case EPuzzleState::Activate:
		// Check deactivation condition.
		if (OverlappingMagic->ElementComponent->GetElementName() == ElementComponent->GetElementWeakness())
		{
			UE_LOG(LogTemp, Log, TEXT("PuzzleAssetDirectBASE: Deactivated"));
			DeactivatePuzzleAsset();
			OnDirectDeactivated.Broadcast(this);
		}
		break;

	case EPuzzleState::Deactivate:
		// Check activation condition.
		if (OverlappingMagic->ElementComponent->GetElementName() == ElementComponent->GetElementName())
		{
			UE_LOG(LogTemp, Log, TEXT("PuzzleAssetDirectBASE: Activated"));
			ActivatePuzzleAsset();
			OnDirectActivated.Broadcast(this);
		}
		break;
	}
}

void AMIR_PuzzleAssetDirectBASE::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	HandleOverlap(OtherActor);
}
