// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_KeyItem.generated.h"

UENUM(Blueprintable)
/* An enumeration of the game's Key Items. */
enum class EMIR_KeyItem : uint8
{
	// The Mirror Staff key item, which grants the copy ability.
	MirrorStaff       UMETA(DisplayName = "MirrorStaff",
						    ToolTip = "The Mirror Staff key item, which grants the copy ability."),

	// The Skybound Shoes key item, which grants the jump ability.
	SkyboundShoes     UMETA(DisplayName = "SkyboundShoes",
							ToolTip = "The Skybound Shoes key item, which grants the jump ability."),

	// The Spirit Stole key item, which grants the dash ability.
	SpiritStole       UMETA(DisplayName = "SpiritStole",
							ToolTip = "The Spirit Stole key item, which grants the dash ability."),

	// A generic Key Gemstone used to increment a counter (TODO: Deprecate).
	KeyGemstone       UMETA(DisplayName = "KeyGemstone",
							ToolTip = "A generic Key Gemstone used to increment a counter (TODO: Deprecate)."),

	// The red Key Gemstone, found in the Lava Chamber and serves as one of four keys to the Final Door.
	KeyGemstoneRed    UMETA(DisplayName = "KeyGemstoneRed",
							ToolTip = "The red Key Gemstone, found in the Lava Chamber and serves as one of four keys to the Final Door."),

	// The blue Key Gemstone, found in the Abandoned Aqueducts and serves as one of four keys to the Final Door.
	KeyGemstoneBlue   UMETA(DisplayName = "KeyGemstoneBlue",
							ToolTip = "The blue Key Gemstone, found in the Abandoned Aqueducts and serves as one of four keys to the Final Door."),

	// The green Key Gemstone, found in the Verdant Grotto and serves as one of four keys to the Final Door.
	KeyGemstoneGreen  UMETA(DisplayName = "KeyGemstoneGreen",
							ToolTip = "The green Key Gemstone, found in the Verdant Grotto and serves as one of four keys to the Final Door."),

	// The yellow Key Gemstone, found in the Cavern of Mirrors and serves as one of four keys to the Final Door.
	KeyGemstoneYellow UMETA(DisplayName = "KeyGemstoneYellow",
							ToolTip = "The yellow Key Gemstone, found in the Cavern of Mirrors and serves as one of four keys to the Final Door."),

};

/**
 *
 */
 //class MIRAANDTHEMIRRORSTAFF_API EMIR_KeyItem
 //{
 //public:
 //	EMIR_KeyItem();
 //	~EMIR_KeyItem();
 //};
