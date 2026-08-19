// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_PuzzleAssetDirectBASE::AMIR_PuzzleAssetDirectBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	RootComponent = CollisionComponent;

	Mesh->SetupAttachment(RootComponent);

	// Set up Element Component
	ElementComponent = CreateDefaultSubobject<UMIR_ElementComponent>(TEXT("ElementComponent"));
}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetDirectBASE::BeginPlay()
{
	Super::BeginPlay();

	OnDirectActivated.AddDynamic(this, &AMIR_PuzzleAssetDirectBASE::FindDirectBase);

	switch (PuzzleState)
	{
	case EPuzzleState::Activate:
		ActivateDirect();
		break;
	case EPuzzleState::Deactivate:
		DeactivateDirect();
		break;
	default:
		break;
	}

	/*CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_PuzzleAssetDirectBASE::
														   OnDirectBaseOverlap);

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Silver, TEXT("DirectBASE setup."));

	UE_LOG(LogTemp, Warning, TEXT("Overlap Event Bound: %s"),
		CollisionComponent->OnComponentBeginOverlap.IsBound() ? TEXT("Yes") : TEXT("No"));*/
}

// Called every frame
void AMIR_PuzzleAssetDirectBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMIR_PuzzleAssetDirectBASE::HandleOverlap(AActor* OtherActor)
{
	// ========== VALIDATE OTHERACTOR =============================================================

	if (!OtherActor)
	{
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Silver,
			FString::Printf(TEXT("DirectBASE collision with %s."), *OtherActor->GetName()));
	}

	// ========== HANDLE ACTIVATION/DEACTIVATION ==================================================

	AMIR_MagicBASE* OverlappingMagic = Cast<AMIR_MagicBASE>(OtherActor);

	if (!OverlappingMagic)
	{
		return;
	}

	if (PuzzleState == EPuzzleState::Activate)  // Active state
	{
		// Check deactivation condition.
		if (OverlappingMagic->ElementComponent->GetElementName() == ElementComponent->GetElementWeakness())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Turquoise,
				FString::Printf(TEXT("DirectBASE deactivation.")));
			PuzzleState = EPuzzleState::Deactivate;
			DeactivateDirect();
		}
	}
	else  // Deactive state
	{
		// Check activation condition.
		if (OverlappingMagic->ElementComponent->GetElementName() == ElementComponent->GetElementName())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta,
				FString::Printf(TEXT("DirectBASE activation.")));
			PuzzleState = EPuzzleState::Activate;
			ActivateDirect();
		}
	}

}
