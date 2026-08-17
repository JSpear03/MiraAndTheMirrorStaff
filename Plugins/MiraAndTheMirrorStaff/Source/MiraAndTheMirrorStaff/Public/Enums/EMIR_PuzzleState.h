// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_PuzzleState.generated.h"

/*
* An enumeration of the Puzzle State for the actors inherited by the Puzzle Asset Indirect Base.
* 
* - Activate: Allows puzzle asset to follow an action. 
* - Deactivate: Does nothing by default or stops a puzzle action. 
*/
UENUM(Blueprintable)
enum class EPuzzleState : uint8 
{
	Activate    UMETA(DisplayName = "Activate", ToolTip = "Allows puzzle asset to follow an action."),
	Deactivate    UMETA(DisplayName = "Deactivate", ToolTip = "Stops puzzle action and do nothing."),
};


/**
 * 
 */
//class MIRAANDTHEMIRRORSTAFF_API EMIR_PuzzleState
//{
//public:
//	EMIR_PuzzleState();
//	~EMIR_PuzzleState();
//};
