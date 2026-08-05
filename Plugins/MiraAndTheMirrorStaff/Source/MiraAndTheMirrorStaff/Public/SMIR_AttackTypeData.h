// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_AttackTypes.h"
#include "SMIR_AttackTypeData.generated.h"


/**
 * Generic struct used to define variables needed for its child structs.
 */
USTRUCT(BlueprintType)
struct FDefaultTypeStruct
{
	GENERATED_BODY()

	// The name of this Attack Type.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ToolTip = "The name of this Attack Type."))
		EAttackTypes AttackTypeName = EAttackTypes::None;

	// The damage value associated with this Attack Type.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 4, UIMin = 0, UIMax = 4,
			  ToolTip = "The damage value associated with this Attack Type."))
		int32 Damage = 0;

	// The mana-consumption value associated with this Attack Type.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 8, UIMin = 0, UIMax = 8,
			  ToolTip = "The mana-consumption value associated with this Attack Type."))
		int32 ManaConsumption = 0;

};


/**
 * Struct defining damage and mana-consumption values for the "Melee" Attack Type.
 */
USTRUCT(BlueprintType)
struct FMeleeStruct : public FDefaultTypeStruct
{
	GENERATED_BODY()

	// FMeleeStruct constructor sets default values for each variable.
	FMeleeStruct()
	{
		AttackTypeName = EAttackTypes::Melee;
		Damage = 2;
		ManaConsumption = 2;
	}
};


/**
 * Struct defining damage and mana-consumption values for the "Ranged" Attack Type.
 */
USTRUCT(BlueprintType)
struct FRangedStruct : public FDefaultTypeStruct
{
	GENERATED_BODY()

	// FRangedStruct constructor sets default values for each variable.
	FRangedStruct()
	{
		AttackTypeName = EAttackTypes::Ranged;
		Damage = 1;
		ManaConsumption = 1;
	}
};


/**
 * Struct defining damage and mana-consumption values for the "AoE" Attack Type.
 */
USTRUCT(BlueprintType)
struct FAoEStruct : public FDefaultTypeStruct
{
	GENERATED_BODY()

	// FAoEStruct constructor sets default values for each variable.
	FAoEStruct()
	{
		AttackTypeName = EAttackTypes::AoE;
		Damage = 4;
		ManaConsumption = 3;
	}
};


/**
 * 
 */
//class MIRAANDTHEMIRRORSTAFF_API SMIR_AttackTypeData
//{
//public:
//	SMIR_AttackTypeData();
//	~SMIR_AttackTypeData();
//};
