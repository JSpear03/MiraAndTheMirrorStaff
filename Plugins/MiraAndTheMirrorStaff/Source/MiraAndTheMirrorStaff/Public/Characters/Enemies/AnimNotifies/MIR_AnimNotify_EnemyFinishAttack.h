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
UCLASS(const, hidecategories=Object, collapsecategories, meta=(DisplayName="Enemy: Finish Attack"))
class MIRAANDTHEMIRRORSTAFF_API UMIR_AnimNotify_EnemyFinishAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	UMIR_AnimNotify_EnemyFinishAttack();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
