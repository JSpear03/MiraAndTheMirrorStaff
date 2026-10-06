// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MIR_MapFunctionLibrary.h"
#include "UI/MIR_MapTileMask.h"
#include "Save/MIR_SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

static bool MatchesRoom(FName VisitedRoom, FName TargetID, const FString& AreaPrefix = TEXT(""))
{
	if (VisitedRoom.IsNone() || TargetID.IsNone())
	{
		return false;
	}

	const FString VisitedStr = VisitedRoom.ToString();
	const FString TargetStr = TargetID.ToString();

	// 1. Save Room special handling (global rooms like L_MIR_SaveRoomA)
	const bool bIsSaveRoom = VisitedStr.Contains(TEXT("SaveRoom"), ESearchCase::IgnoreCase) ||
	                         VisitedStr.Contains(TEXT("SavePlatform"), ESearchCase::IgnoreCase);
	const bool bTargetIsSave = TargetStr.Contains(TEXT("Save"), ESearchCase::IgnoreCase);

	if (bIsSaveRoom && bTargetIsSave)
	{
		return true;
	}

	// 2. Boss Room alias handling
	const bool bTargetIsBoss = TargetStr.Contains(TEXT("Boss"), ESearchCase::IgnoreCase);
	if (bTargetIsBoss)
	{
		if (VisitedStr.Contains(TEXT("Boss"), ESearchCase::IgnoreCase) ||
		    (AreaPrefix.Equals(TEXT("VG"), ESearchCase::IgnoreCase) && VisitedStr.Contains(TEXT("C1"), ESearchCase::IgnoreCase)) ||
		    (AreaPrefix.Equals(TEXT("AA"), ESearchCase::IgnoreCase) && VisitedStr.Contains(TEXT("F1"), ESearchCase::IgnoreCase)) ||
		    (AreaPrefix.Equals(TEXT("IS"), ESearchCase::IgnoreCase) && VisitedStr.Contains(TEXT("B2"), ESearchCase::IgnoreCase)) ||
		    (AreaPrefix.Equals(TEXT("LC"), ESearchCase::IgnoreCase) && VisitedStr.Contains(TEXT("D1"), ESearchCase::IgnoreCase)))
		{
			return true;
		}
	}

	// If AreaPrefix is specified, VisitedRoom must match that area prefix (unless it's a global room like SaveRoom)
	if (!AreaPrefix.IsEmpty() && !bIsSaveRoom && !VisitedStr.Contains(AreaPrefix, ESearchCase::IgnoreCase))
	{
		return false;
	}

	if (VisitedRoom == TargetID)
	{
		return true;
	}

	if (VisitedStr.Contains(TargetStr, ESearchCase::IgnoreCase) ||
		TargetStr.Contains(VisitedStr, ESearchCase::IgnoreCase))
	{
		return true;
	}

	return false;
}

bool UMIR_MapFunctionLibrary::IsRoomDiscovered(const UObject* WorldContextObject, FName RoomID, const TArray<FName>& AssociatedRoomIDs, bool bRequireAllRooms, const FString& AreaPrefix)
{
	if (!WorldContextObject)
	{
		return false;
	}

	UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GI)
	{
		return false;
	}

	UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>();
	if (!SaveSubsystem)
	{
		return false;
	}

	const TArray<FName>& VisitedRooms = SaveSubsystem->GetVisitedRooms();
	if (VisitedRooms.Num() == 0)
	{
		return false;
	}

	TArray<FName> AllIDsToCheck;
	if (!RoomID.IsNone())
	{
		AllIDsToCheck.Add(RoomID);
	}
	for (const FName& Id : AssociatedRoomIDs)
	{
		if (!Id.IsNone())
		{
			AllIDsToCheck.AddUnique(Id);
		}
	}

	if (AllIDsToCheck.Num() == 0)
	{
		return false;
	}

	if (bRequireAllRooms)
	{
		for (const FName& TargetID : AllIDsToCheck)
		{
			bool bFound = false;
			for (const FName& Visited : VisitedRooms)
			{
				if (MatchesRoom(Visited, TargetID, AreaPrefix))
				{
					bFound = true;
					break;
				}
			}
			if (!bFound)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (const FName& TargetID : AllIDsToCheck)
		{
			for (const FName& Visited : VisitedRooms)
			{
				if (MatchesRoom(Visited, TargetID, AreaPrefix))
				{
					return true;
				}
			}
		}
		return false;
	}
}

void UMIR_MapFunctionLibrary::UpdateTileWidgetFog(const UObject* WorldContextObject, UWidget* TileWidget, FName RoomID, const TArray<FName>& AssociatedRoomIDs, bool bRequireAllRooms, bool bInvertFog, const FString& AreaPrefix)
{
	if (!TileWidget)
	{
		return;
	}

	const bool bRevealed = IsRoomDiscovered(WorldContextObject, RoomID, AssociatedRoomIDs, bRequireAllRooms, AreaPrefix);

	if (bInvertFog)
	{
		TileWidget->SetVisibility(bRevealed ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	else
	{
		TileWidget->SetVisibility(bRevealed ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
}

void UMIR_MapFunctionLibrary::UpdateGridColumnFog(const UObject* WorldContextObject, const TArray<UWidget*>& ColumnWidgets, const FString& ColumnLetter, int32 StartRowNumber, const FString& AreaPrefix)
{
	if (!WorldContextObject || ColumnLetter.IsEmpty())
	{
		return;
	}

	for (int32 Index = 0; Index < ColumnWidgets.Num(); ++Index)
	{
		UWidget* Widget = ColumnWidgets[Index];
		if (!Widget)
		{
			continue;
		}

		int32 RowNum = StartRowNumber + Index;
		FName CoordinateID(*FString::Printf(TEXT("%s%d"), *ColumnLetter, RowNum));

		TArray<FName> EmptyAssociated;
		UpdateTileWidgetFog(WorldContextObject, Widget, CoordinateID, EmptyAssociated, false, false, AreaPrefix);
	}
}

void UMIR_MapFunctionLibrary::RefreshAllMapTileMasksInPanel(UPanelWidget* ContainerPanel)
{
	if (!ContainerPanel)
	{
		return;
	}

	const int32 ChildCount = ContainerPanel->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; ++i)
	{
		UWidget* Child = ContainerPanel->GetChildAt(i);
		if (!Child)
		{
			continue;
		}

		if (UMIR_MapTileMask* TileMask = Cast<UMIR_MapTileMask>(Child))
		{
			TileMask->RefreshFogVisibility();
		}
		else if (UPanelWidget* SubPanel = Cast<UPanelWidget>(Child))
		{
			RefreshAllMapTileMasksInPanel(SubPanel);
		}
	}
}
