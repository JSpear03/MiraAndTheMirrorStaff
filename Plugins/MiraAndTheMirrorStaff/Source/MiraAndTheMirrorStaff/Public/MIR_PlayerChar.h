// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_CharacterBASE_M.h"
#include "MIR_ManaComponent.h"
#include "MIR_PlayerChar.generated.h"

/**
 * The character that the player will control in this game.
 * Implements gameplay-context inputs, etc.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PlayerChar : public AMIR_CharacterBASE_M
{
	GENERATED_BODY()

public:
	// Represents the game's Mana system.
	UPROPERTY(EditAnywhere, Category = "Mana")
	UMIR_ManaComponent* ManaComponent;

	// Sets default values for Player Character's properties.
	AMIR_PlayerChar();
};
