// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Characters/Enemies/MIR_EnemyManager.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_EnemyManager::AMIR_EnemyManager()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up TriggerBox.
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(500.0, 1100.0, 500.0));
	RootComponent = TriggerBox;

}

// Called when the game starts or when spawned
void AMIR_EnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// Check for any enemies that were already defeated and destroyed on BeginPlay.
	int32 AlreadyDefeatedCount = 0;
	for (AMIR_EnemyBASE* CurrentEnemy : EnemyArray)
	{
		if (!CurrentEnemy || CurrentEnemy->IsActorBeingDestroyed())
		{
			AlreadyDefeatedCount++;
			continue;
		}

		CurrentEnemy->OnEnemyDied.AddDynamic(this, &AMIR_EnemyManager::VerifyEnemyInArray);
	}

	EnemyDeathCount = AlreadyDefeatedCount;

	// Bind TriggerBox's `OnComponentBeginOverlap()` to `OnOverlapBegin()`.
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMIR_EnemyManager::OnOverlapBegin);
}

// Called every frame
void AMIR_EnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/****************************************************************************************
 * TRIGGER
 ****************************************************************************************/

void AMIR_EnemyManager::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Continue only if this is the first trigger in this actor's lifespan.
	if (!bDoOnce) { return; }

	// Continue only if all enemies in `EnemyArray` are valid.
	// This prevents an issue where IPAs cannot be deactivated if the enemies don't exist.
	/* The currently evaluated enemy in `EnemyArray`. */
	for (const AMIR_EnemyBASE* CurrentEnemy : EnemyArray)
	{
		if (!CurrentEnemy) { return; }
	}
	
	// The currently possessed player character.
	AMIR_PlayerChar* PlayerChar = Cast<AMIR_PlayerChar>(OtherActor);

	// Continue only if the player overlapped with this actor.
	if (!OtherActor || !PlayerChar) { return; }

	// If all enemies in this encounter are already defeated, do not trigger gates or music.
	if (EnemyArray.Num() > 0 && EnemyDeathCount >= EnemyArray.Num())
	{
		bDoOnce = false;
		return;
	}

	PlayBossMusic();

	// Activate/deactivate appropriately.
	if (bIsForwardActivation && EnemyDeathCount < EnemyArray.Num())
	{
		DeactivateAllIndirects();
	}
	else
	{
		ActivateAllIndirects();
	}

	// Prevent repeated triggers.
	bDoOnce = false;
}

/****************************************************************************************
 * LINKED ASSETS | ENEMIES
 ****************************************************************************************/

void AMIR_EnemyManager::VerifyEnemyInArray(AMIR_EnemyBASE* EnemyRef)
{
	// Exit if invalid.
	if (!EnemyRef) { return; }

	// Continue only if `EnemyArray` contains the current enemy instance.
	if (!EnemyArray.Contains(EnemyRef)) { return; }

	// Is contained, so increment death counter.
	EnemyDeathCount++;

	// Continue only if all enemies have died.
	if (EnemyDeathCount < EnemyArray.Num()) { return; }

	StopBossMusic();

	// Do IPA activation/deactivation based on mode.
	if (bIsForwardActivation)
	{
		ActivateAllIndirects();
	}
	else
	{
		DeactivateAllIndirects();
	}
}

/****************************************************************************************
 * LINKED ASSETS | INDIRECT PUZZLE ASSETS
 ****************************************************************************************/

void AMIR_EnemyManager::ActivateAllIndirects()
{
	// The current IPA in the `IndirectAssetArray`.
	for (AMIR_PuzzleAssetsIndirectBASE* CurrentIndirect : IndirectAssetArray)
	{
		CurrentIndirect->ActivatePuzzleAsset();
	}
}

void AMIR_EnemyManager::DeactivateAllIndirects()
{
	// The current IPA in the `IndirectAssetArray`.
	for (AMIR_PuzzleAssetsIndirectBASE* CurrentIndirect : IndirectAssetArray)
	{
		CurrentIndirect->DeactivatePuzzleAsset();
	}
}
