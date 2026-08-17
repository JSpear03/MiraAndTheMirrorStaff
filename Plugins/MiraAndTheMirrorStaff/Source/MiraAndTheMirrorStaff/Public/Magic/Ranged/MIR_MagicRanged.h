// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/MIR_MagicBASE.h"
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Magic|Movement")
		class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The amount of time until the summoned magic despawns.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Despawn")
		float MagicLifespan = 10.0f;

	// Represents whether certain logic in `Tick()` has executed once.
	bool bDoOnce = false;
};
