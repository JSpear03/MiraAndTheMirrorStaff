// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Enums/EMIR_KeyItem.h"
#include "MIR_SaveGame.generated.h"

/** Per-room persistent record. Only SaveGame-flagged fields are serialized to disk. */
USTRUCT(BlueprintType)
struct FMIR_LevelRecord
{
	GENERATED_BODY()

	// Represents whether the player has cleared the game.
	// TODO: Determine whether to add a hard mode (unlikely).
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Clear")
	bool bGameCleared = false;
};

/**
 * The on-disk, gameplay-only progress for Mira and the Mirror Staff. Lives in a
 * save slot and is owned by UMIR_SaveSubsystem (the save manager).
 */
UCLASS(BlueprintType)
class MIRAANDTHEMIRRORSTAFF_API UMIR_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Bumped when the layout changes so old saves can be migrated/discarded.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Version")
		int32 SaveVersion = 1;

	// Specifies the major area that should be opened.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Spawn")
		TSoftObjectPtr<UWorld> SpawnArea = nullptr;

	// Specifies a Player Spawn to spawn the player at when loading.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Spawn")
		FName SpawnPoint = TEXT("None");

	// Specifies the player's total health based on how many Health-Ups they've collected.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Stats",
			  meta = (ClampMin = 8, UIMin = 8, ClampMax = 16, UIMax = 16))
		int32 TotalHealth = 8;

	// Specifies the player's total mana based on how many Health-Ups they've collected.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Stats",
			  meta = (ClampMin = 8, UIMin = 8, ClampMax = 16, UIMax = 16))
		int32 TotalMana = 8;

	// Specifies whether the player has obtained the Mirror Staff (Copy ability).
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Equipment")
		bool bHasMirrorStaff = false;

	// Specifies whether the player has obtained the Skybound Shoes (Jump ability).
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Equipment")
		bool bHasSkyboundShoes = false;

	// Specifies whether the player has obtained the Spirit Stole (Dash ability).
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Equipment")
		bool bHasSpiritStole = false;

	// Specifies whether the player has obtained the red Key Gemstone.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Gemstones")
		bool bHasRedKeystone = false;

	// Specifies whether the player has obtained the blue Key Gemstone.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Gemstones")
		bool bHasBlueKeystone = false;

	// Specifies whether the player has obtained the green Key Gemstone.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Gemstones")
		bool bHasGreenKeystone = false;

	// Specifies whether the player has obtained the yellow Key Gemstone.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Player Items|Gemstones")
		bool bHasYellowKeystone = false;

	// List of unique pickup IDs that have been collected so they don't respawn.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Pickups")
		TArray<FName> CollectedPickups;

	// List of unique boss IDs that have been defeated so they don't respawn.
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Save|Bosses")
		TArray<FName> DefeatedBosses;
};
