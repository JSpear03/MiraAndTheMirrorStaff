// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_CharacterBASE_M.h"
#include "MIR_CopiableComponent.h"
#include "MIR_EnemyBASE.generated.h"

/**
 *  Base class for all enemies in the game. 
 *  Includes logic such as spawning dust, as well as checking if enemy can be copied.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_EnemyBASE : public AMIR_CharacterBASE_M
{
	GENERATED_BODY()
	
public:
	// Sets default value damage when player contacts enemy. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Contact Damage")
		int32 ContactDamage = 1;

	// Implements copy ability. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copy")
		UMIR_CopiableComponent* CopiableComponent;

	// Sets default values for Base Enemy. 
	AMIR_EnemyBASE();
};
