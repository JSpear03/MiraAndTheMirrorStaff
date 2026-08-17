// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_Elements.generated.h"


/**
 * An Enumeration of all Elements used in the game.
 * - None: A default value that is not meant to be used in-game.
 * - Nothing: An Element without any advantages nor disadvantages. No properties.
 * - Flame: An Element utilizing fire. Has burning properties.
 * - Ice: An Element utilizing ice. Has freezing properties.
 * - Thunder: An Element utilizing lightning. Has electrifying properties.
 */
UENUM(BlueprintType)
enum class EElements : uint8
{
	None    UMETA(DisplayName = "None",    ToolTip = "A default value that is not meant to be used in-game."),
	Nothing UMETA(DisplayName = "Nothing", ToolTip = "An Element without any advantages nor disadvantages. No properties."),
	Flame   UMETA(DisplayName = "Flame",   ToolTip = "An Element utilizing fire. Has burning properties."),
	Ice     UMETA(DisplayName = "Ice",     ToolTip = "An Element utilizing ice. Has freezing properties."),
	Thunder UMETA(DisplayName = "Thunder", ToolTip = "An Element utilizing lightning. Has electrifying properties.")
};


/**
 * An Enumeration of all Element properties used in the game.
 * - None: Specifies no Element properties.
 * - Burn: Specifies a burning property for Flame.
 * - Freeze: Specifies a freezing property for Ice.
 * - Electrocute: Specifies an electrocuting property for Thunder.
 */
UENUM(BlueprintType)
enum class EElementProperties : uint8
{
	None        UMETA(DisplayName = "None",        ToolTip = "Specifies no Element properties."),
	Burn        UMETA(DisplayName = "Burn",        ToolTip = "Specifies a burning property for Flame."),
	Freeze      UMETA(DisplayName = "Freeze",      ToolTip = "Specifies a freezing property for Ice."),
	Electrocute UMETA(DisplayName = "Electrocute", ToolTip = "Specifies an electrocuting property for Thunder.")
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
