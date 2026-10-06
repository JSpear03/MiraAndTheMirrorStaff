// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MIR_MapTileMask.h"
#include "Save/MIR_SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

UMIR_MapTileMask::UMIR_MapTileMask()
{
	// Default to solid black blackout tile
	SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMIR_MapTileMask::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	RefreshFogVisibility();
}

void UMIR_MapTileMask::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (UWorld* World = GetWorld())
	{
		if (World->IsGameWorld() && !bBoundToSubsystem)
		{
			if (UMIR_SaveSubsystem* SaveSubsystem = GetSaveSubsystem())
			{
				SaveSubsystem->OnRoomVisited.AddUniqueDynamic(this, &UMIR_MapTileMask::HandleRoomVisited);
				bBoundToSubsystem = true;
			}
		}
	}

	RefreshFogVisibility();
}

void UMIR_MapTileMask::ReleaseSlateResources(bool bReleaseChildren)
{
	if (bBoundToSubsystem)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GetSaveSubsystem())
		{
			SaveSubsystem->OnRoomVisited.RemoveDynamic(this, &UMIR_MapTileMask::HandleRoomVisited);
		}
		bBoundToSubsystem = false;
	}

	Super::ReleaseSlateResources(bReleaseChildren);
}

UMIR_SaveSubsystem* UMIR_MapTileMask::GetSaveSubsystem() const
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			return GI->GetSubsystem<UMIR_SaveSubsystem>();
		}
	}
	return nullptr;
}

static bool CheckRoomMatches(FName VisitedRoom, FName TargetID, const FString& AreaPrefix = TEXT(""))
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

	// If an AreaPrefix is specified, VisitedRoom must match that area prefix (unless it's a global room like SaveRoom)
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

bool UMIR_MapTileMask::ShouldBeRevealed() const
{
	UMIR_SaveSubsystem* SaveSubsystem = GetSaveSubsystem();
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
				if (CheckRoomMatches(Visited, TargetID, AreaPrefix))
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
				if (CheckRoomMatches(Visited, TargetID, AreaPrefix))
				{
					return true;
				}
			}
		}
		return false;
	}
}

void UMIR_MapTileMask::RefreshFogVisibility()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!World->IsGameWorld())
	{
		if (bPreviewRevealedInEditor)
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		return;
	}

	const bool bRevealed = ShouldBeRevealed();

	if (bInvertFog)
	{
		SetVisibility(bRevealed ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(bRevealed ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
}

void UMIR_MapTileMask::HandleRoomVisited(FName VisitedRoomName)
{
	RefreshFogVisibility();
}
