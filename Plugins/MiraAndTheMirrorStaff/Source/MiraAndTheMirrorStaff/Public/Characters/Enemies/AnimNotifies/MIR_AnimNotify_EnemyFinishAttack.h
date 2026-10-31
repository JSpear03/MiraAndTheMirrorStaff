// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MIR_AnimNotify_EnemyFinishAttack.generated.h"

/**
 * Anim Notify placed near the end/recovery of an enemy's attack animation sequence
 * to cleanly finish the attack and return the AI to idle/movement without hardcoded delays.
 */
UCLASS(const, hidecategories = Object, collapsecategories,
	   meta = (DisplayName = "Enemy: Finish Attack"))
class MIRAANDTHEMIRRORSTAFF_API UMIR_AnimNotify_EnemyFinishAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	/**
	 * If in the editor, changes the notify color.
	 */
	UMIR_AnimNotify_EnemyFinishAttack();

	/**
	 * Attempts to finish an enemy's attack animation.
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
