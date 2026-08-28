// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAssets/Manager/MIR_PuzzleAssetManager.h"
#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
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

	DetermineForwardActivation();	// Determine the management mode.

	// Bind DPA delegates to `VerifyDirectInArray()`.
	/* The current DPA in the `DirectAssetArray`. */
	for (AMIR_PuzzleAssetDirectBASE* CurrentDirect : DirectAssetArray)
	{
		if (!CurrentDirect)
		{
			continue;
		}

		CurrentDirect->OnDirectActivated.AddDynamic(this, &AMIR_PuzzleAssetManager::VerifyDirectInArray);
		CurrentDirect->OnDirectDeactivated.AddDynamic(this, &AMIR_PuzzleAssetManager::VerifyDirectInArray);
	}
}

// Called when the game ends or when destroyed.
void AMIR_PuzzleAssetManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ClearAutoTimer();

	// Unbind DPA delegates from `VerifyDirectInArray()`.
	/* The current DPA in the `DirectAssetArray`. */
	for (AMIR_PuzzleAssetDirectBASE* CurrentDirect : DirectAssetArray)
	{
		if (!CurrentDirect)
		{
			continue;
		}

		CurrentDirect->OnDirectActivated.RemoveDynamic(this,&AMIR_PuzzleAssetManager::VerifyDirectInArray);
		CurrentDirect->OnDirectDeactivated.RemoveDynamic(this, &AMIR_PuzzleAssetManager::VerifyDirectInArray);
	}
}

// Called every frame
void AMIR_PuzzleAssetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/****************************************************************************************
 * LINKED ASSETS | DIRECT
 ****************************************************************************************/

void AMIR_PuzzleAssetManager::VerifyDirectInArray(AMIR_PuzzleAssetDirectBASE* DirectAssetRef)
{
	// The current DirectBASE object reference.
	AMIR_PuzzleAssetDirectBASE* CurrentDirect = Cast<AMIR_PuzzleAssetDirectBASE>(DirectAssetRef);

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("Checking direct asset"));

	if (!CurrentDirect)
	{
		return;
	}

	// Check whether `CurrentDirect` is contained in the `DirectContainer`.
	if (DirectAssetArray.Contains(CurrentDirect))
	{
		// Is contained, so try IPA activation/deactivation based on mode.
		if (bIsForwardActivation)
		{
			TryIndirectActivation();
		}
		else
		{
			TryIndirectDeactivation();
		}
	}
}

bool AMIR_PuzzleAssetManager::FindAllDirectsInState(EPuzzleState InState)
{
	// Represents how many DPAs in the `DirectAssetArray` are in the specified state.
	int32 StateCounter = 0;

	// Count how many DPAs are in the specified state.
	/* The current DPA in the `DirectAssetArray`. */
	for (AMIR_PuzzleAssetDirectBASE* CurrentDirect : DirectAssetArray)
	{
		// Check whether the current DPA is in the specified state.
		if (CurrentDirect->GetCurrentState() == InState)
		{
			StateCounter++;
		}
	}

	// Determine return value.
	//if (StateCounter == DirectAssetArray.Num())
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	// Determine return value.
	return (StateCounter == DirectAssetArray.Num()) ? true : false;
}

/****************************************************************************************
 * LINKED ASSETS | INDIRECT
 ****************************************************************************************/

void AMIR_PuzzleAssetManager::TryIndirectActivation()
{
	if (FindAllDirectsInState(EPuzzleState::Activate))
	{
		IndirectAsset->ActivatePuzzleAsset();
		ClearAutoTimer();
	}
	else
	{
		if (IndirectAsset->GetCurrentState() == EPuzzleState::Deactivate)
		{
			ExecuteAutoTimer();
		}
		else
		{
			IndirectAsset->DeactivatePuzzleAsset();
		}
	}
}

void AMIR_PuzzleAssetManager::TryIndirectDeactivation()
{
	if (FindAllDirectsInState(EPuzzleState::Deactivate))
	{
		IndirectAsset->DeactivatePuzzleAsset();
		ClearAutoTimer();
	}
	else
	{
		if (IndirectAsset->GetCurrentState() == EPuzzleState::Activate)
		{
			ExecuteAutoTimer();
		}
		else
		{
			IndirectAsset->ActivatePuzzleAsset();
		}
	}
}

/****************************************************************************************
 * TIMER
 ****************************************************************************************/

void AMIR_PuzzleAssetManager::ExecuteAutoTimer()
{
	// Do not execute if Auto Timer is disabled.
	if (!bUseAutoTimer)
	{
		return;
	}

	// Manages the Auto Timer from the world's `TimerManager`.
	FTimerManager& AutoTimerManager = GetWorld()->GetTimerManager();

	// The delegate to execute upon elapsion of the Auto Timer.
	FTimerDelegate AutoTimerDelegate;

	// Bind the proper function to `AutoTimerDelegate` based on `bIsForwardActivation`.
	if (bIsForwardActivation)
	{
		AutoTimerDelegate.BindUFunction(this, FName("HandleAutoDeactivation"));
	}
	else
	{
		AutoTimerDelegate.BindUFunction(this, FName("HandleAutoActivation"));
	}

	// Commence timer.
	AutoTimerManager.SetTimer(AutoTimerHandle, AutoTimerDelegate, AutoTimerDuration, false);
}

void AMIR_PuzzleAssetManager::ClearAutoTimer()
{
	if (AutoTimerHandle.IsValid())
	{
		AutoTimerHandle.Invalidate();
	}
}

void AMIR_PuzzleAssetManager::HandleAutoActivation()
{
	if (!FindAllDirectsInState(EPuzzleState::Deactivate))
	{
		// Activate all DPAs in the `DirectAssetArray`, then also the IPA.
		/* The current DPA in the `DirectAssetArray`. */
		for (AMIR_PuzzleAssetDirectBASE* CurrentDirect : DirectAssetArray)
		{
			CurrentDirect->ActivatePuzzleAsset();
		}
		IndirectAsset->ActivatePuzzleAsset();
	}
}

void AMIR_PuzzleAssetManager::HandleAutoDeactivation()
{
	if (!FindAllDirectsInState(EPuzzleState::Activate))
	{
		// Deactivate all DPAs in the `DirectAssetArray`, then also the IPA.
		/* The current DPA in the `DirectAssetArray`. */
		for (AMIR_PuzzleAssetDirectBASE* CurrentDirect : DirectAssetArray)
		{
			CurrentDirect->DeactivatePuzzleAsset();
		}
		IndirectAsset->DeactivatePuzzleAsset();
	}
}

/****************************************************************************************
 * MODE
 ****************************************************************************************/

void AMIR_PuzzleAssetManager::DetermineForwardActivation()
{
	if (!IndirectAsset)
	{
		return;
	}
	else
	{
		if (IndirectAsset->GetInitialState() == EPuzzleState::Activate)
		{
			// If the IPA is already active, then the PAM needs to operate in reverse.
			bIsForwardActivation = false;
		}
		else
		{
			// If the IPA is already inactive, then the PAM needs to operate forward.
			bIsForwardActivation = true;
		}
	}
}
