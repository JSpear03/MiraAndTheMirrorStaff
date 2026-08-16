// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PuzzleAssetManager.h"
#include "MIR_PuzzleAssetDirectBASE.h"
#include "MIR_PuzzleAssetsIndirectBASE.h"
#include "MIR_PlayerChar.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_PuzzleAssetManager::AMIR_PuzzleAssetManager()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMIR_PuzzleAssetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMIR_PuzzleAssetManager::FindDirectBase(AMIR_PuzzleAssetManager* DirectAsset)
{
	// The current DirectBASE object reference.
	AMIR_PuzzleAssetDirectBASE* CurrentDirect = Cast<AMIR_PuzzleAssetDirectBASE>(DirectAsset);

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("Checking direct asset"));

	// Check whether `CurrentDirect` is contained in the `DirectContainer`.
	if (DirectContainer.Contains(CurrentDirect))
	{
		// Is contained, so turn it on.
		CurrentDirect->bDidTurnOn = true;
	}
	else
	{
		// Not contained, so do nothing.
		return;
	}

	// Finally, check whether all objects are on to determine timer activation.
	CheckActiveInDirect(DirectAsset);
}

void AMIR_PuzzleAssetManager::CheckActiveInDirect(AMIR_PuzzleAssetManager* DirectAsset)
{
	// Check whether all objects are on.
	if (FindAllDirectActive() == DirectContainer.Num())
	{
		// True, so activate the linked IndirectBASE object and commence timer.
		/* The current DirectBASE object reference. */
		AMIR_PuzzleAssetDirectBASE* CurrentDirect = Cast<AMIR_PuzzleAssetDirectBASE>(DirectAsset);

		// Commence activation of the linked IndirectBASE and the timer.
		CurrentDirect->LinkedActor->CallActivateDelegate();
		SetActiveTimer(2);
	}
}

int32 AMIR_PuzzleAssetManager::FindAllDirectActive()
{
	// Represents how many referenced DirectBASE objects are turned on.
	int32 TurnOnCounter = 0;

	// Check the ON status of each referenced object.
	for (AMIR_PuzzleAssetManager* Item : DirectContainer)  // The current object in the `DirectContainer`.
	{
		// The current DirectBASE object reference.
		AMIR_PuzzleAssetDirectBASE* CurrentDirect = Cast<AMIR_PuzzleAssetDirectBASE>(Item);

		// Check whether the current object is on. If so, increment counter.
		if (CurrentDirect->bDidTurnOn)
		{
			TurnOnCounter++;
		}
	}

	return TurnOnCounter;
}

void AMIR_PuzzleAssetManager::SetActiveTimer(float Duration)
{
	// Do nothing if the `Duration` is 0.
	if (Duration == 0)
	{
		return;
	}

	// ========== PREPARE VARIABLES ==========

	FTimerHandle CooldownTimerHandle;  // The handle for this timer, making it distinct.
	const UWorld* World = GetWorld();  // A reference to the current world.

	// Manages the timer from the world's `TimerManager`.
	FTimerManager& TimerManager = World->GetTimerManager();

	// The currently possessed Character.
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	
	// The currently possessed PlayerCharacter, casted to AMIR_PlayerChar.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);

	// ========== CREATE DELEGATE ==========

	// This timer's delegate, which handles the deactivation of all DirectBASE objects and
	// their linked IndirectBASE.
	const FTimerDelegate& CooldownTimerDelegate = FTimerDelegate::CreateLambda([this, &TimerManager, 
		&CooldownTimerHandle, &PlayerCharacter]
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire Active Cooldown"));

			TimerManager.ClearTimer(CooldownTimerHandle);

			// Only executes, if not all elements are active. 
			if (FindAllDirectActive() != DirectContainer.Num())
			{
				// Deactivate all DirectBASE objects and their linked IndirectBASE.
				for (AMIR_PuzzleAssetManager* Item : DirectContainer)  // The current object in the `DirectContainer`.
				{
					// The current DirectBASE object reference.
					AMIR_PuzzleAssetDirectBASE* CurrentDirect = Cast<AMIR_PuzzleAssetDirectBASE>(Item);

					CurrentDirect->LinkedActor->DeactivateIndirect();  // Deactivates IndirectBASE.
					CurrentDirect->DeactivateDirect(PlayerCharacter);  // Deactivates DirectBASE.
				}
			}
		}
	);

	// ========== HANDLE TIMER ==========

	// Commence timer.
	TimerManager.SetTimer(CooldownTimerHandle, CooldownTimerDelegate, Duration, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Active Timer is running"));

	// Perform peri-timer operations.
	DuringDuration();
}

void AMIR_PuzzleAssetManager::DuringDuration()
{
	// Check whether all objects are on.
	if (FindAllDirectActive() == DirectContainer.Num())
	{
		for (AMIR_PuzzleAssetManager* Item : DirectContainer)  // The current object in the `DirectContainer`.
		{
			// The current DirectBASE object reference.
			AMIR_PuzzleAssetDirectBASE* CurrentDirect = Cast<AMIR_PuzzleAssetDirectBASE>(Item);

			CurrentDirect->LinkedActor->CallActivateDelegate();  // Activates IndirectBASE.
		}
	}
}




