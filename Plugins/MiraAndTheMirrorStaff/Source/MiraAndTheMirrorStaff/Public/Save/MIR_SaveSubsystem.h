// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MIR_SaveSubsystem.generated.h"

class UMIR_SaveGame;
class USaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMIR_OnSaveLoaded, UMIR_SaveGame*, Save);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMIR_OnSaveWritten, bool, bSuccess);

/**
 * The save manager (another "manager, not GameMode"). Owns the loaded UMIR_SaveGame,
 * loads it synchronously on startup (the payload is tiny), and writes asynchronously
 * during play so a save never hitches the frame. Gameplay records results here; menus
 * read unlock/best-time state here. Survives level streaming as a GameInstance subsystem.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API UMIR_SaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the subsystem, loading a save file per `SaveUserIndex` or creating a
	 * new one otherwise. Afterward, it broadcasts `CurrentSave`.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Deinitializes the subsystem, setting `CurrentSave` to null. */
	virtual void Deinitialize() override;

	// When the system has loaded the save file.
	UPROPERTY(BlueprintAssignable, Category = "Save")
	FMIR_OnSaveLoaded OnSaveLoaded;

	// When the system has written to the save file.
	UPROPERTY(BlueprintAssignable, Category = "Save")
	FMIR_OnSaveWritten OnSaveWritten;

	/**
	 * The current in-memory save (created if it doesn't exist yet).
	 * 
	 * @returns A save-game instance.
	 */
	UFUNCTION(BlueprintPure, Category = "Save", meta = (ReturnDisplayName = "SaveGame"))
	UMIR_SaveGame* GetSave();

	/** Write the current save to its slot asynchronously. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveAsync();

	/** Reload the save from disk asynchronously (or fresh if none exists). */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadAsync();

	/** Wipe progress back to a fresh save and write it. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void ResetSave();

	/**
	 * Returns whether a save file exists.
	 * 
	 * @returns Whether said file exists.
	 */
	UFUNCTION(BlueprintPure, Category = "Save", meta = (ReturnDisplayName = "SaveExists?"))
	bool DoesSaveExist() const;

	/**
	 * Returns the name of the current save-file slot.
	 * 
	 * @returns A static, const FString with said name.
	 */
	static const FString& GetSlotName();

	// The currently specified save slot.
	static constexpr int32 SaveUserIndex = 0;

	/**
	 * Sets all fields in the current save game.
	 * 
	 * @param NewTotalHealth Corresponds to the player's `TotalHealth` variable.
	 * @param NewTotalMana Corresponds to the player's `TotalManaPower` variable.
	 * @param bStaffObtained Whether the Mirror Staff is obtained.
	 * @param bShoesObtained Whether the Skybound Shoes are obtained.
	 * @param bStoleObtained Whether the Spirit Stole is obtained.
	 * @param bRedKeystoneObtained Whether the red Key Gemstone is obtained.
	 * @param bBlueKeystoneObtained Whether the blue Key Gemstone is obtained.
	 * @param bGreenKeystoneObtained Whether the green Key Gemstone is obtained.
	 * @param bYellowKeystoneObtained Whether the yellow Key Gemstone is obtained.
	 * @param NewSpawnArea The major area to be opened (e.g., Verdant Grotto).
	 * @param NewSpawnPoint Specifies a PlayerStart to spawn at.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save", meta = (AdvancedDisplay = 2))
	void SetAllFields(int32 NewTotalHealth = 8, int32 NewTotalMana = 8, bool bStaffObtained = false,
		bool bShoesObtained = false, bool bStoleObtained = false, bool bRedKeystoneObtained = false,
		bool bBlueKeystoneObtained = false, bool bGreenKeystoneObtained = false,
		bool bYellowKeystoneObtained = false, TSoftObjectPtr<UWorld> NewSpawnArea = nullptr,
		FName NewSpawnPoint = TEXT("None"));

	// Sets the player's spawn area in the current save game.

	/**
	 * Sets the player's spawn area in the current save game.
	 * 
	 * @param NewSpawnArea The major area to be opened (e.g., Verdant Grotto).
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Spawn")
	void SetSpawnArea(TSoftObjectPtr<UWorld> NewSpawnArea);

	/**
	 * Sets the player's spawn point in the current save game.
	 * 
	 * @param NewSpawnPoint Specifies a PlayerStart to spawn at.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Spawn")
	void SetSpawnPoint(FName NewSpawnPoint = TEXT("None"));

	/**
	 * Sets the player's total health in the current save game.
	 *
	 * @param NewTotalHealth Corresponds to the player's `TotalHealth` variable.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Stats")
	void SetTotalHealth(int32 NewTotalHealth = 8);

	/**
	 * Sets the player's total mana in the current save game.
	 *
	 * @param NewTotalMana Corresponds to the player's `TotalManaPower` variable.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Stats")
	void SetTotalMana(int32 NewTotalMana = 8);

	/**
	 * Sets the player's possession of the Mirror Staff in the current save game.
	 *
	 * @param bPossessionStatus Whether the Mirror Staff is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Equipment")
	void SetHasMirrorStaff(bool bPossessionStatus = false);

	/**
	 * Sets the player's possession of the Skybound Shoes in the current save game.
	 *
	 * @param bPossessionStatus Whether the Skybound Shoes is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Equipment")
	void SetHasSkyboundShoes(bool bPossessionStatus = false);

	/**
	 * Sets the player's possession of the Spirit Stole in the current save game.
	 *
	 * @param bPossessionStatus Whether the Spirit Stole is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Equipment")
	void SetHasSpiritStole(bool bPossessionStatus = false);

	/**
	 * Sets the player's possession of the red Key Gemstone in the current save game.
	 *
	 * @param bPossessionStatus Whether the red Key Gemstone is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Gemstones")
	void SetHasRedKeystone(bool bPossessionStatus = false);

	/**
	 * Sets the player's possession of the blue Key Gemstone in the current save game.
	 *
	 * @param bPossessionStatus Whether the blue Key Gemstone is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Gemstones")
	void SetHasBlueKeystone(bool bbossessionStatus = false);

	/**
	 * Sets the player's possession of the green Key Gemstone in the current save game.
	 *
	 * @param bPossessionStatus Whether the green Key Gemstone is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Gemstones")
	void SetHasGreenKeystone(bool bPossessionStatus = false);

	/**
	 * Sets the player's possession of the yellow Key Gemstone in the current save game.
	 *
	 * @param bPossessionStatus Whether the yellow Key Gemstone is obtained.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save|Player Items|Gemstones")
	void SetHasYellowKeystone(bool bPossessionStatus = false);

private:
	// Creates a new save file, and then returns it.
	UMIR_SaveGame* CreateNewSave();

	/**
	 * Validates the passed `LoadedSave` and assigns it to `CurrentSave` if so.
	 * Otherwise, validates `CurrentSave` and creates a new save file if not so.
	 * 
	 * @param Slot The specified save slot (currently unused).
	 * @param UserIndex The specified user index (currently unused).
	 * @param LoadedSave The loaded save file to validate.
	 */
	void HandleAsyncLoadCompleted(const FString& Slot, int32 UserIndex, USaveGame* LoadedSave);

	/**
	 * Sets `bSaveInFlight` to false (re-enabling async saving) and broadcasts success.
	 * 
	 * @param Slot The specified save slot (currently unused).
	 * @param UserIndex The specified user index (currently unused).
	 * @param bSuccess Whether the async save was successful (broadcasted on `OnSaveWritten()`).
	 */
	void HandleAsyncSaveCompleted(const FString& Slot, int32 UserIndex, bool bSuccess);

	// The current save file.
	UPROPERTY(Transient)
	TObjectPtr<UMIR_SaveGame> CurrentSave;

	// Represents whether an async load is in progress.
	bool bSaveInFlight = false;
};
