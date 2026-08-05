// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_AttackTypes.generated.h"


/**
 * An Enumeration of all Attack Types used in the game.
 * - None: A default value that is not meant to be used in game.
 * - Melee: Close-ranged attacks; typically swords.
 * - Ranged: Projectile-based attacks.
 * - AoE: Close- to mid-ranged attacks; an explosion around the summoner.
 */
UENUM(BlueprintType)
enum class EAttackTypes : uint8
{
	None   UMETA(DisplayName = "None"),
	Melee  UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
	AoE    UMETA(DisplayName = "AoE")
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
