// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMIR_Elements.h"
#include "SMIR_ElementData.generated.h"


/**
 * Generic struct used to define variables needed for its child structs.
 */
USTRUCT(BlueprintType)
struct FDefaultElementStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EElements ElementName = EElements::Nothing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterial* MainColor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* ElementSymbol = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<EElements> ElementStrength = { EElements::Nothing };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<EElements> ElementWeakness = { EElements::Nothing };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<EElements> ElementNeutralities = { EElements::Nothing };

	// Represents the Element's properties (its interactions with the game world).
	// Replace with a Tag(?) at some point.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ElementProperty = "None";
};


/**
 * Struct defining values for the "Nothing" Element.
 */
USTRUCT(BlueprintType)
struct FNothingStruct : public FDefaultElementStruct
{
	GENERATED_BODY()

	FNothingStruct()
	{
		ElementName = EElements::Nothing;
		MainColor = nullptr;
		ElementSymbol = nullptr;
		ElementStrength = { EElements::Nothing };
		ElementWeakness = { EElements::Nothing };
		ElementNeutralities = { EElements::Nothing };
		ElementProperty = "None";
	}
};


/**
 * Struct defining values for the "Flame" Element.
 */
USTRUCT(BlueprintType)
struct FFlameStruct : public FDefaultElementStruct
{
	GENERATED_BODY()

	FFlameStruct()
	{
		ElementName = EElements::Flame;
		MainColor = nullptr;
		ElementSymbol = nullptr;
		ElementStrength = { EElements::Ice};
		ElementWeakness = { EElements::Thunder };
		ElementNeutralities = { EElements::Nothing, EElements::Flame };
		ElementProperty = "Burn";
	}
};


/**
 * Struct defining values for the "Ice" Element.
 */
USTRUCT(BlueprintType)
struct FIceStruct : public FDefaultElementStruct
{
	GENERATED_BODY()

	FIceStruct()
	{
		ElementName = EElements::Ice;
		MainColor = nullptr;
		ElementSymbol = nullptr;
		ElementStrength = { EElements::Thunder };
		ElementWeakness = { EElements::Flame };
		ElementNeutralities = { EElements::Nothing, EElements::Ice };
		ElementProperty = "Freeze";
	}
};


/**
 * Struct defining values for the "Thunder" Element.
 */
USTRUCT(BlueprintType)
struct FThunderStruct : public FDefaultElementStruct
{
	GENERATED_BODY()

	FThunderStruct()
	{
		ElementName = EElements::Thunder;
		MainColor = nullptr;
		ElementSymbol = nullptr;
		ElementStrength = { EElements::Flame };
		ElementWeakness = { EElements::Ice };
		ElementNeutralities = { EElements::Nothing, EElements::Thunder };
		ElementProperty = "Electrocute";
	}
};


/**
 *
 */
 //class MIRAANDTHEMIRRORSTAFF_API SMIR_ElementData
 //{
 //public:
 //	SMIR_ElementData();
 //	~SMIR_ElementData();
 //};
