// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/MIR_MagicBASE.h"
#include "MIR_MagicMelee.generated.h"

class UMIR_AttackTypeComponent;

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_MagicMelee : public AMIR_MagicBASE
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMIR_MagicMelee();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The amount of time until the summoned magic despawns.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Despawn")
		float MagicLifespan = 2.0f;

	// The initial x-axis rotation (relative).
	const float InitialRotationX = -90.0;

	// The initial rotation upon swinging (relative).
	const float InitialRotationZ = -90.0f;

	// The final rotation upon swinging (relative).
	const float FinalRotationZ = 90.0f;

	/**
	 * @brief Updates the magic's rotation via linear interpolation (lerp).
	 *
	 * Performs a lerp using the passed `Alpha` value to calculate a new z-axis rotation
	 * for the Billboard Component. `InitialRotationZ` and `FinalRotationZ` are passed to
	 * the lerp's `A` and `B` parameters, dictating the magic's final in-game rotation.
	 * Accordingly, the Billboard Component visibly rotates only on the z-axis.
	 * Afterward, this value is passed to the Billboard Component's relative rotation.
	 *
	 * @param Alpha The alpha to use when performing the lerp.
	 *
	 * @returns N/A
	 */
	UFUNCTION(BlueprintCallable, Category = "Magic|Collision", meta = (KeyWords = "Swing"))
		void SwingMagic(const float& Alpha);
};