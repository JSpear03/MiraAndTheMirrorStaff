// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "MIR_MapTileMask.generated.h"

class UMIR_SaveSubsystem;

/**
 * A blackout / fog tile mask widget placed over unified area map textures.
 * Automatically queries UMIR_SaveSubsystem to collapse when its assigned room has been visited.
 */
UCLASS(meta = (DisplayName = "Map Tile Mask"))
class MIRAANDTHEMIRRORSTAFF_API UMIR_MapTileMask : public UImage
{
	GENERATED_BODY()

public:
	UMIR_MapTileMask();

	/** Primary Room ID to check (e.g. "A2", "L_MIR_VGRoomA2"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Fog")
	FName RoomID;

	/** Optional area prefix (e.g. "VG", "AA", "CM", "LC", "IS") to match zone rooms when using short codes like "A2". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Fog")
	FString AreaPrefix;

	/** Additional Room IDs if this tile spans multiple rooms or sub-levels. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Fog")
	TArray<FName> AssociatedRoomIDs;

	/** If true, all specified rooms must be visited. If false, any visited room reveals the tile. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Fog")
	bool bRequireAllRooms = false;

	/** If true, widget is visible when visited and collapsed when unvisited (e.g. highlight overlay). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Fog")
	bool bInvertFog = false;

	/** If true in the UMG editor designer preview, simulate the tile being revealed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Fog|Debug")
	bool bPreviewRevealedInEditor = false;

	/** Returns true if the associated room(s) are visited in the save file. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Map Fog")
	bool ShouldBeRevealed() const;

	/** Re-evaluates visited state and updates visibility accordingly. */
	UFUNCTION(BlueprintCallable, Category = "Map Fog")
	void RefreshFogVisibility();

	virtual void SynchronizeProperties() override;
	virtual void OnWidgetRebuilt() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	UFUNCTION()
	void HandleRoomVisited(FName VisitedRoomName);

	UMIR_SaveSubsystem* GetSaveSubsystem() const;

private:
	bool bBoundToSubsystem = false;
};
