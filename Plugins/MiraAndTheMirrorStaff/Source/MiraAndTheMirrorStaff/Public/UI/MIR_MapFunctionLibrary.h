// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/PanelWidget.h"
#include "MIR_MapFunctionLibrary.generated.h"

/**
 * Blueprint utility helpers for driving map fog of war, blackout tiles, and room discovery.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API UMIR_MapFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Checks whether a specific room or coordinate (e.g. "A2", "L_MIR_VGRoomA2") has been visited. */
	UFUNCTION(BlueprintPure, Category = "Map Fog", meta = (WorldContext = "WorldContextObject"))
	static bool IsRoomDiscovered(const UObject* WorldContextObject, FName RoomID, const TArray<FName>& AssociatedRoomIDs, bool bRequireAllRooms = false, const FString& AreaPrefix = TEXT(""));

	/**
	 * Updates any UWidget (such as an existing UImage tile in a column) to be collapsed if discovered, or visible if undiscovered.
	 */
	UFUNCTION(BlueprintCallable, Category = "Map Fog", meta = (WorldContext = "WorldContextObject"))
	static void UpdateTileWidgetFog(const UObject* WorldContextObject, UWidget* TileWidget, FName RoomID, const TArray<FName>& AssociatedRoomIDs, bool bRequireAllRooms = false, bool bInvertFog = false, const FString& AreaPrefix = TEXT(""));

	/**
	 * Updates a column array of tile widgets (e.g. Col1Images) where array elements correspond to Row 1, Row 2, etc.
	 * Constructs room queries such as "A1", "A2", etc., and collapses visited tiles.
	 *
	 * @param ColumnWidgets Array of widgets (images) for this column.
	 * @param ColumnLetter Letter representing the column (e.g. "A", "B", "C").
	 * @param StartRowNumber Starting row number (default 1).
	 * @param AreaPrefix Optional area prefix (e.g. "VG", "AA", "CM") to disambiguate coordinates across zones.
	 */
	UFUNCTION(BlueprintCallable, Category = "Map Fog", meta = (WorldContext = "WorldContextObject"))
	static void UpdateGridColumnFog(const UObject* WorldContextObject, const TArray<UWidget*>& ColumnWidgets, const FString& ColumnLetter, int32 StartRowNumber = 1, const FString& AreaPrefix = TEXT(""));

	/**
	 * Refreshes all UMIR_MapTileMask widgets placed inside the given panel widget (recursively).
	 */
	UFUNCTION(BlueprintCallable, Category = "Map Fog")
	static void RefreshAllMapTileMasksInPanel(UPanelWidget* ContainerPanel);
};
