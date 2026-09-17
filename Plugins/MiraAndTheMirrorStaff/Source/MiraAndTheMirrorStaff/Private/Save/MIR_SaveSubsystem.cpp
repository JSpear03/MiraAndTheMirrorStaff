// Fill out your copyright notice in the Description page of Project Settings.

#include "Save/MIR_SaveSubsystem.h"
#include "Save/MIR_SaveGame.h"
#include "Kismet/GameplayStatics.h"

const FString& UMIR_SaveSubsystem::GetSlotName()
{
	// The name of the slot to return.
	static const FString SlotName = TEXT("MIR_Progress");
	return SlotName;
}

/*********************************************************************************************
* INITIALIZATION & DEINITIALIZATION
*********************************************************************************************/

void UMIR_SaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load synchronously on startup — the payload is tiny and this avoids a race where
	// GetSave() creates a fresh save before an async load lands.
	if (UGameplayStatics::DoesSaveGameExist(GetSlotName(), SaveUserIndex))
	{
		CurrentSave = Cast<UMIR_SaveGame>(UGameplayStatics::LoadGameFromSlot(GetSlotName(),
										  SaveUserIndex));
	}

	// Create a new save if `CurrentSave` is invalid.
	if (!CurrentSave)
	{
		CurrentSave = CreateNewSave();
	}

	// Broadcast `CurrentSave`.
	OnSaveLoaded.Broadcast(CurrentSave);
}

void UMIR_SaveSubsystem::Deinitialize()
{
	CurrentSave = nullptr;
	Super::Deinitialize();
}

/*********************************************************************************************
* SAVE-GAME SETTERS | ALL FIELDS
*********************************************************************************************/

void UMIR_SaveSubsystem::SetAllFields(int32 NewTotalHealth, int32 NewTotalMana, bool bStaffObtained,
	bool bShoesObtained, bool bStoleObtained, bool bRedKeystoneObtained, bool bBlueKeystoneObtained,
	bool bGreenKeystoneObtained, bool bYellowKeystoneObtained, TSoftObjectPtr<UWorld> NewSpawnArea,
	FName NewSpawnPoint)
{
	// Set player stats.
	SetTotalHealth(NewTotalHealth);
	SetTotalMana(NewTotalMana);

	// Set player items - equipment.
	SetHasMirrorStaff(bStaffObtained);
	SetHasSkyboundShoes(bShoesObtained);
	SetHasSpiritStole(bStoleObtained);

	// Set player items - Key Gemstones.
	SetHasRedKeystone(bRedKeystoneObtained);
	SetHasBlueKeystone(bBlueKeystoneObtained);
	SetHasGreenKeystone(bGreenKeystoneObtained);
	SetHasYellowKeystone(bYellowKeystoneObtained);

	// Set player spawn.
	SetSpawnArea(NewSpawnArea);
	SetSpawnPoint(NewSpawnPoint);
}

/*****************************************************
* SAVE-GAME SETTERS | SPAWN
*****************************************************/

void UMIR_SaveSubsystem::SetSpawnArea(TSoftObjectPtr<UWorld> NewSpawnArea)
{
	GetSave()->SpawnArea = NewSpawnArea;
}

void UMIR_SaveSubsystem::SetSpawnPoint(FName NewSpawnPoint)
{
	GetSave()->SpawnPoint = NewSpawnPoint;
}

/*****************************************************
* SAVE-GAME SETTERS | PLAYER STATS
*****************************************************/

void UMIR_SaveSubsystem::SetTotalHealth(int32 NewTotalHealth)
{
	GetSave()->TotalHealth = NewTotalHealth;
}

void UMIR_SaveSubsystem::SetTotalMana(int32 NewTotalMana)
{
	GetSave()->TotalMana = NewTotalMana;
}

/*****************************************************
* SAVE-GAME SETTERS | PLAYER ITEMS | EQUIPMENT
*****************************************************/

void UMIR_SaveSubsystem::SetHasMirrorStaff(bool bPossessionStatus)
{
	GetSave()->bHasMirrorStaff = bPossessionStatus;
}

void UMIR_SaveSubsystem::SetHasSkyboundShoes(bool bPossessionStatus)
{
	GetSave()->bHasSkyboundShoes = bPossessionStatus;
}

void UMIR_SaveSubsystem::SetHasSpiritStole(bool bPossessionStatus)
{
	GetSave()->bHasSpiritStole = bPossessionStatus;
}

/*****************************************************
* SAVE-GAME SETTERS | PLAYER ITEMS | KEY GEMSTONES
*****************************************************/

void UMIR_SaveSubsystem::SetHasRedKeystone(bool bPossessionStatus)
{
	GetSave()->bHasRedKeystone = bPossessionStatus;
}

void UMIR_SaveSubsystem::SetHasBlueKeystone(bool bPossessionStatus)
{
	GetSave()->bHasBlueKeystone = bPossessionStatus;
}

void UMIR_SaveSubsystem::SetHasGreenKeystone(bool bPossessionStatus)
{
	GetSave()->bHasGreenKeystone = bPossessionStatus;
}

void UMIR_SaveSubsystem::SetHasYellowKeystone(bool bPossessionStatus)
{
	GetSave()->bHasYellowKeystone = bPossessionStatus;
}

/*********************************************************************************************
* DATA MANAGEMENT
*********************************************************************************************/

UMIR_SaveGame* UMIR_SaveSubsystem::CreateNewSave()
{
	return Cast<UMIR_SaveGame>(UGameplayStatics::CreateSaveGameObject(UMIR_SaveGame::StaticClass()));
}

UMIR_SaveGame* UMIR_SaveSubsystem::GetSave()
{
	// Create a new save if `CurrentSave` is invalid.
	if (!CurrentSave)
	{
		CurrentSave = CreateNewSave();
	}
	return CurrentSave;
}

bool UMIR_SaveSubsystem::DoesSaveExist() const
{
	return UGameplayStatics::DoesSaveGameExist(GetSlotName(), SaveUserIndex);
}

void UMIR_SaveSubsystem::SaveAsync()
{
	// The current save-game instance.
	UMIR_SaveGame* Save = GetSave();

	// Exit if `Save` is invalid or an async save is already in progress(?).
	if (!Save || bSaveInFlight)
	{
		return;
	}

	bSaveInFlight = true;

	// Perform async save.
	FAsyncSaveGameToSlotDelegate Done;  // Delgate for completed async save.
	Done.BindUObject(this, &UMIR_SaveSubsystem::HandleAsyncSaveCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(Save, GetSlotName(), SaveUserIndex, Done);
}

void UMIR_SaveSubsystem::LoadAsync()
{
	FAsyncLoadGameFromSlotDelegate Done;  // Delegate for completed async load.
	Done.BindUObject(this, &UMIR_SaveSubsystem::HandleAsyncLoadCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(GetSlotName(), SaveUserIndex, Done);
}

void UMIR_SaveSubsystem::ResetSave()
{
	CurrentSave = CreateNewSave();
	SaveAsync();
	OnSaveLoaded.Broadcast(CurrentSave);
}

/*****************************************************
* DATA MANAGEMENT | ASYNC COMPLETION
*****************************************************/

void UMIR_SaveSubsystem::HandleAsyncLoadCompleted(const FString& /*Slot*/, int32 /*UserIndex*/,
	USaveGame* LoadedSave)
{
	// Validate the loaded save and `CurrentSave`.
	/* A loaded save-game instance. */
	if (UMIR_SaveGame* Loaded = Cast<UMIR_SaveGame>(LoadedSave))
	{
		CurrentSave = Loaded;
	}
	else if (!CurrentSave)
	{
		CurrentSave = CreateNewSave();
	}

	OnSaveLoaded.Broadcast(CurrentSave);
}

void UMIR_SaveSubsystem::HandleAsyncSaveCompleted(const FString& /*Slot*/, int32 /*UserIndex*/,
	bool bSuccess)
{
	bSaveInFlight = false;
	OnSaveWritten.Broadcast(bSuccess);
}
