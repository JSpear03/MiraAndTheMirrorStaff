// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/MIR_MagicBASE.h"
#include "Components/TimelineComponent.h"
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

	// The Timeline Component for the expansion animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Timeline",
		meta = (ToolTip = "The Timeline Component for the expansion animation."))
	TObjectPtr<UTimelineComponent> ExpansionTimeline;

protected:
	/**
	 * @brief Expands the magic's size via linear interpolation (lerp).
	 * 
	 * Performs a lerp using the passed `Alpha` value to calculate a new half-height and radius
	 * for both collision components. `FinalSize` (accessible in the Blueprint's Details) is
	 * passed to the lerp's `B` parameter, dictating the magic's final in-game size.
	 * Afterward, this value is passed to each collision's half-height and radius in that precise
	 * order (because the radius is clamped to the half-height).
	 * 
	 * @param Alpha The alpha to use when performing the lerp.
	 * 
	 * @returns N/A
	 */
	void MagicExpansion(const float& Alpha);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The amount of time until the summoned magic despawns.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Despawn")
		float MagicLifespan = 1.0f;

	// The initial size of both collision components.
	const float InitialSize = 1.0f;

	// The the final size of both collision components post-expansion.
	UPROPERTY(EditAnywhere, Category = "Magic|Collision")
		float FinalSize = 300.0f;

	// An array of both collision components.
	const TArray<UCapsuleComponent*> TCapsuleArray = { CollisionComponent, IceCollisionComponent };

	// A curve for `ExpansionTimeline`.
	// If no curve is assigned in the Unreal editor, one will be made natively.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Timeline|Curve")
		TObjectPtr<UCurveFloat> ExpansionCurve = nullptr;

private:
	// The alpha to be fed into `ExpansionTimeline`.
	FOnTimelineFloat ExpansionAlpha;

	// A function signature for the Complete event of `ExpansionTimeline`.
	FOnTimelineEvent ExpansionCompleteEvent;

protected:

	/**
	 * Called when `ExpansionTimeline` updates.
	 */
	UFUNCTION()
		void ExpansionUpdate(float Alpha);

	/**
	 * Called once `ExpansionTimeline` is complete.
	 */
	UFUNCTION()
		void ExpansionComplete();

private:
	/**
	 * Sets up `ExpansionTimeline`.
	 */
	void SetUpTimeline();

	/**
	 * Sets up `ExpansionCurve` if one is not assigned in the Unreal editor.
	 */
	void SetUpCurve();
};
