// Fill out your copyright notice in the Description page of Project Settings.


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

	// Bind EnemyBASE delegates to `VerifyEnemyInArray()`.
	/* The current EnemyBASE in the `EnemyArray`. */
	for (AMIR_EnemyBASE* CurrentEnemy : EnemyArray)
	{
		if (!CurrentEnemy) { continue; }

		CurrentEnemy->OnEnemyDied.AddDynamic(this, &AMIR_EnemyManager::VerifyEnemyInArray);
	}

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
	
	// The currently possessed player character.
	AMIR_PlayerChar* PlayerChar = Cast<AMIR_PlayerChar>(OtherActor);

	// Continue only if the player overlapped with this actor.
	if (!OtherActor || !PlayerChar) { return; }

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
	// The current EnemyBASE object reference.
	AMIR_EnemyBASE* CurrentEnemy = Cast<AMIR_EnemyBASE>(EnemyRef);

	// Exit if invalid.
	if (!CurrentEnemy) { return; }

	// Continue only if `EnemyArray` contains the current enemy instance.
	if (!EnemyArray.Contains(CurrentEnemy)) { return; }

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
