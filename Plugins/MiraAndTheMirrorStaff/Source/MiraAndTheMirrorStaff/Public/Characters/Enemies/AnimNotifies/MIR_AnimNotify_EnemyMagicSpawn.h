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
UCLASS(const, hidecategories=Object, collapsecategories, meta=(DisplayName="Enemy: Spawn Magic"))
class MIRAANDTHEMIRRORSTAFF_API UMIR_AnimNotify_EnemyMagicSpawn : public UAnimNotify
{
	GENERATED_BODY()

public:
	UMIR_AnimNotify_EnemyMagicSpawn();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
