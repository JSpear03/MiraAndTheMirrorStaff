// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_MagicBASE.h"
#include "MIR_MagicAOE.generated.h"

class UMIR_AttackTypeComponent;

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_MagicAOE : public AMIR_MagicBASE
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMIR_MagicAOE();

	// Expands the spell's radius over time using linear interpolation.
	void SpellExpansion(float InitRadius, float MaxRadius, float LerpTime, float DeltaTime);
};
