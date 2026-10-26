// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "EMIR_PuzzleState.generated.h"

/**
* An enumeration of the Puzzle States for puzzle assets.
* 
* - Activate: Allows puzzle asset to follow an action. 
* - Deactivate: Does nothing by default or stops a puzzle action. 
*/
UENUM(Blueprintable)
enum class EPuzzleState : uint8
{
	// An activated state where the puzzle asset can operate, commencing operations.
	Activate    UMETA(DisplayName = "Activate",
					  ToolTip = "An activated state where the puzzle asset can operate, commencing operations."),

	// A deactivated state where the puzzle asset cannot operate, ceasing operations.
	Deactivate  UMETA(DisplayName = "Deactivate",
					  ToolTip = "A deactivated state where the puzzle asset cannot operate, ceasing operations."),
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
