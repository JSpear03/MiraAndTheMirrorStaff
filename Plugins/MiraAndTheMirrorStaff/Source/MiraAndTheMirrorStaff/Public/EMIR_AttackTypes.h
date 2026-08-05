// Fill out your copyright notice in the Description page of Project Settings.

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
	None   UMETA(DisplayName = "None",   ToolTip = "A default value that is not meant to be used in-game."),
	Melee  UMETA(DisplayName = "Melee",  ToolTip = "Specifies close-ranged, physical attacks."),
	Ranged UMETA(DisplayName = "Ranged", ToolTip = "Specifies long-ranged, projectile-based attacks."),
	AoE    UMETA(DisplayName = "AoE",    ToolTip = "Specifies close- to mid-ranged, explosive attacks.")
};


/**
 * An Enumeration of all Attack Types properties used in the game.
 * - None: A default value that is not meant to be used in game.
 * - Cut: Specifies a cutting property, used to destroy things like vines.
 */
UENUM(BlueprintType)
enum class EAttackProperties : uint8
{
	None  UMETA(DisplayName = "None", ToolTip = "Specifies no Attack Type properties."),
	Cut   UMETA(DisplayName = "Cut",  ToolTip = "Specifies a cutting property, used to destroy things like vines."),
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
