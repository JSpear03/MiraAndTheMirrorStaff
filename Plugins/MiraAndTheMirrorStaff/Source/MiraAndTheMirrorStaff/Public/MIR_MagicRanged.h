// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_MagicBASE.h"
#include "MIR_MagicRanged.generated.h"

class UMIR_AttackTypeComponent;

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_MagicRanged : public AMIR_MagicBASE
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMIR_MagicRanged();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Magic|Collision")
		class USphereComponent* IceCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Magic|Movement")
		class UProjectileMovementComponent* ProjectileMovement;
};
