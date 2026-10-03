// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/MIR_MagicBASE.h"
#include "Components/TimelineComponent.h"
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

	// The Timeline Component for the swing animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Timeline",
		meta = (ToolTip = "The Timeline Component for the swing animation."))
	TObjectPtr<UTimelineComponent> SwingTimeline;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The amount of time until the summoned magic despawns.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Despawn")
		float MagicLifespan = 0.1f;

	// The initial x-axis rotation (relative).
	const float InitialRotationX = -90.0;

	// The initial rotation upon swinging (relative).
	const float InitialRotationZ = -90.0f;

	// The final rotation upon swinging (relative).
	const float FinalRotationZ = 90.0f;

	// A curve for `SwingTimeline`.
	// If no curve is assigned in the Unreal editor, one will be made natively.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Timeline|Curve")
		TObjectPtr<UCurveFloat> SwingCurve = nullptr;

private:
	// The alpha to be fed into `SwingTimeline`.
	FOnTimelineFloat SwingAlpha;

	// A function signature for the Complete event of `SwingTimeline`.
	FOnTimelineEvent SwingCompleteEvent;

protected:

	/**
	 * Called when `SwingTimeline` updates.
	 */
	UFUNCTION()
		void SwingUpdate(float Alpha);

	/**
	 * Called once `SwingTimeline` is complete.
	 */
	UFUNCTION()
		void SwingComplete();

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
	void SwingMagic(const float& Alpha);

private:
	/**
	 * Sets up `SwingTimeline`.
	 */
	void SetUpTimeline();

	/**
	 * Sets up `SwingCurve` if one is not assigned in the Unreal editor.
	 */
	void SetUpCurve();
};