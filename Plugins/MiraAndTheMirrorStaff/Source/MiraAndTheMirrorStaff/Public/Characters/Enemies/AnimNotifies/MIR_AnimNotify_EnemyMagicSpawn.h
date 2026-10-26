// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MIR_AnimNotify_EnemyMagicSpawn.generated.h"

/**
 * Anim Notify placed on an enemy's attack animation sequence (e.g. skeleton slash)
 * to trigger the magic/projectile spawn precisely at that animation frame.
 */
UCLASS(const, hidecategories = Object, collapsecategories,
	   meta = (DisplayName = "Enemy: Spawn Magic"))
class MIRAANDTHEMIRRORSTAFF_API UMIR_AnimNotify_EnemyMagicSpawn : public UAnimNotify
{
	GENERATED_BODY()

public:
	/**
	 * If in the editor, changes the notify color.
	 */
	UMIR_AnimNotify_EnemyMagicSpawn();

	/**
	 * Attempts to spawn magic in an enemy's attack animation.
	 *
	 * @param MeshComp The skeletal mesh playing the animation.
	 * @param Animation The animation sequence that triggered the notify.
	 * @param EventReference Contextual info about the triggering animation event.
	 */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	/**
	 * Returns a custom name for the notify.
	 */
	virtual FString GetNotifyName_Implementation() const override;
};
