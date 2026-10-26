// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_AttackTypes.generated.h"

/**
 * An Enumeration of all Attack Types used in the game.
 * - None: A default value that is not meant to be used in-game.
 * - Melee: Close-ranged attacks; typically swords.
 * - Ranged: Projectile-based attacks.
 * - AoE: Close- to mid-ranged attacks; an explosion around the summoner.
 */
UENUM(BlueprintType)
enum class EAttackTypes : uint8
{
	// A default value that is not meant to be used in-game.
	None   UMETA(DisplayName = "None",   ToolTip = "A default value that is not meant to be used in-game."),

	// Specifies close-ranged, physical attacks.
	Melee  UMETA(DisplayName = "Melee",  ToolTip = "Specifies close-ranged, physical attacks."),

	// Specifies long-ranged, projectile-based attacks.
	Ranged UMETA(DisplayName = "Ranged", ToolTip = "Specifies long-ranged, projectile-based attacks."),

	// Specifies close- to mid-ranged, explosive attacks.
	AoE    UMETA(DisplayName = "AoE",    ToolTip = "Specifies close- to mid-ranged, explosive attacks.")
};


/**
 * An Enumeration of all Attack Types properties used in the game.
 * - None: A default value that is not meant to be used in game.
 * - Cut: Specifies a cutting property, used to destroy things like vines.
 * - Copy: Specifies a copying property, used to copy magic.
 */
UENUM(BlueprintType)
enum class EAttackProperties : uint8
{
	// Specifies no Attack Type properties.
	None  UMETA(DisplayName = "None", ToolTip = "Specifies no Attack Type properties."),

	// Specifies a cutting property, used to destroy things like vines.
	Cut   UMETA(DisplayName = "Cut",  ToolTip = "Specifies a cutting property, used to destroy things like vines."),
	
	// Specifies a copying property, used to copy magic.
	Copy  UMETA(DisplayName = "Copy", ToolTip = "Specifies a copying property, used to copy magic."),
};


/**
 * An Enumeration of all Attack Subtypes used in the game.
 * Meant to be used in `SetAttackTypeDetails()` for simplicity.
 */
UENUM(BlueprintType)
enum class EAttackSubtypes : uint8
{
	// No subtype.
	None    UMETA(DisplayName = "None",   ToolTip = "No subtype."),

	// Allows the copying of magic. Zero damage and mana consumption.
	Copy    UMETA(DisplayName = "Copy",   ToolTip = "Allows the copying of magic. Zero damage and mana consumption."),

	// Melee subtype with halved damage and no mana consumption.
	Dagger  UMETA(DisplayName = "Dagger", ToolTip = "Melee subtype with halved damage and no mana consumption.")
};


/**
 * 
 */
//class MIRAANDTHEMIRRORSTAFF_API EMIR_AttackTypes
//{
//public:
//	EMIR_AttackTypes();
//	~EMIR_AttackTypes();

//public:
//	EAttackTypes CurrentAttackType = EAttackTypes::Melee;
//};
