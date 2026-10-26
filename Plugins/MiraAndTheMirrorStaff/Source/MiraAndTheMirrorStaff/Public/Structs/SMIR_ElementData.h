// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Enums/EMIR_Elements.h"
#include "SMIR_ElementData.generated.h"

/**
 * Generic struct used to define variables needed for its child structs.
 */
USTRUCT(BlueprintType)
struct FDefaultElementStruct
{
	GENERATED_BODY()

	// The name of this Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Name",
			  ToolTip = "The name of this Element."))
		EElements ElementName = EElements::None;

	// A Linear Color, which specifies a main color, used by this Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "MainColor",
			  ToolTip = "A Linear Color, which specifies a main color, used by this Element."))
		FLinearColor MainColor = FLinearColor::White;

	// A Linear Color, which specifies a sub-color, used by this Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "SubColor",
			  ToolTip = "A Linear Color, which specifies a sub-color, used by this Element."))
		FLinearColor SubColor = FLinearColor::Black;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Property",
			  ToolTip = "The property of this Element."))
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
		MainColor = FLinearColor(0.0f, 1.0f, 0.393f, 1.0f);
		SubColor = FLinearColor(0.0f, 0.042f, 0.017f, 1.0f);
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
		MainColor = FLinearColor(1.0f, 0.147f, 0.002f, 1.0f);
		SubColor = FLinearColor(1.0f, 0.546f, 0.125f, 1.0f);
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
		MainColor = FLinearColor(0.491f, 0.839f, 0.905f, 1.0f);
		SubColor = FLinearColor(0.095f, 0.503f, 0.939f, 1.0f);
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
		MainColor = FLinearColor(1.0f, 0.723f, 0.002f, 1.0f);
		SubColor = FLinearColor(0.93f, 0.823f, 0.287f, 1.0f);
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
