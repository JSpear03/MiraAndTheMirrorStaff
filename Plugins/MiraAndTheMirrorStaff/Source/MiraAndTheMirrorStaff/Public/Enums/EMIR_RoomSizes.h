// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "EMIR_RoomSizes.generated.h"

/**
 * An Enumeration of all room sizes (XY-space) used in the game.
 */
UENUM(BlueprintType)
enum class ERoomSizes : uint8
{
	// Specifies a 1x1 room, which is uniform in size.
	OneByOne	  UMETA(DisplayName = "1x1", ToolTip = "Specifies a 1x1 room, which is uniform in size."),

	// Specifies a 1x2 room, which is longer on the x-axis.
	OneByTwo	  UMETA(DisplayName = "1x2", ToolTip = "Specifies a 1x2 room, which is longer on the x-axis."),

	// Specifies a 2x1 room, which is wider on the y-axis.
	TwoByOne	  UMETA(DisplayName = "2x1", ToolTip = "Specifies a 2x1 room, which is wider on the y-axis."),

	// Specifies a 2x2 room, which is uniform in size.
	TwoByTwo      UMETA(DisplayName = "2x2", ToolTip = "Specifies a 2x2 room, which is uniform in size."),

	// Specifies a 3x3 room, which is uniform in size.
	ThreeByThree  UMETA(DisplayName = "3x3", ToolTip = "Specifies a 3x3 room, which is uniform in size.")
};

/**
 *
 */
 //class MIRAANDTHEMIRRORSTAFF_API EMIR_RoomSizes
 //{
 //public:
 //	EMIR_RoomSizes();
 //	~EMIR_RoomSizes();
 //};
