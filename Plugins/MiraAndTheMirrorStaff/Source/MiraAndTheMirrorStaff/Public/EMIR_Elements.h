// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_Elements.generated.h"


/**
 * An Enumeration of all Elements used in the game.
 * - Nothing: An element without any advantages nor disadvantages. No properties.
 * - Flame: An element utilizing fire. Has burning properties.
 * - Ice: An element utilizing ice. Has freezing properties.
 * - Thunder: An element utilizing lightning. Has electrifying properties.
 */
UENUM(BlueprintType)
enum class EElements : uint8
{
	Nothing UMETA(DisplayName = "Nothing"),
	Flame   UMETA(DisplayName = "Flame"),
	Ice     UMETA(DisplayName = "Ice"),
	Thunder UMETA(DisplayName = "Thunder")
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
