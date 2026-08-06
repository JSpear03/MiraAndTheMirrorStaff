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

	// The name of this Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Name", ToolTip = "The name of this Element."))
		EElements ElementName = EElements::None;

	// The Material, which specifies a main color, used by this Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "MainColor",
			  ToolTip = "The Material, which specifies a main color, used by this Element."))
		UMaterial* MainColor = nullptr;

	// The symbol of this Element, which would be used in the Gameplay UI.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Symbol",
			  ToolTip = "The symbol of this Element, which would be used in the Gameplay UI."))
		UTexture2D* ElementSymbol = nullptr;

	// The Element that *this* Element applies double damage (2×) against.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Strength",
			  ToolTip = "One or more Elements that *this* Element applies double damage (2×) against."))
		EElements ElementStrength = EElements::None;

	// The Elements that *this* Element applies zero damage (0×) against.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Weakness",
			  ToolTip = "One or more Elements that *this* Element applies zero damage (0×) against."))
		EElements ElementWeakness = EElements::None;

	// One or more Elements that *this* Element applies default damage (1×) against.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Neutralities",
			  ToolTip = "One or more Elements that *this* Element applies default damage (1×) against."))
		TArray<EElements> ElementNeutralities = { EElements::None };

	// The property of this Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Property", ToolTip = "The property of this Element."))
		EElementProperties ElementProperty = EElementProperties::None;
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
		ElementStrength = EElements::Nothing;
		ElementWeakness = EElements::Nothing;
		ElementNeutralities = { EElements::Nothing };
		ElementProperty = EElementProperties::None;
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
		ElementStrength = EElements::Ice;
		ElementWeakness = EElements::Thunder;
		ElementNeutralities = { EElements::Nothing, EElements::Flame };
		ElementProperty = EElementProperties::Burn;
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
		ElementStrength = EElements::Thunder;
		ElementWeakness = EElements::Flame;
		ElementNeutralities = { EElements::Nothing, EElements::Ice };
		ElementProperty = EElementProperties::Freeze;
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
		ElementStrength = EElements::Flame;
		ElementWeakness = EElements::Ice;
		ElementNeutralities = { EElements::Nothing, EElements::Thunder };
		ElementProperty = EElementProperties::Electrocute;
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
