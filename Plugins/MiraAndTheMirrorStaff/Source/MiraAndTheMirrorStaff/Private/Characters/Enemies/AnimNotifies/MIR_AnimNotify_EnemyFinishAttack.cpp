// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Characters/Enemies/AnimNotifies/MIR_AnimNotify_EnemyFinishAttack.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Components/MIR_Combat_Component.h"

UMIR_AnimNotify_EnemyFinishAttack::UMIR_AnimNotify_EnemyFinishAttack()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(0, 200, 100, 255);
#endif
}

void UMIR_AnimNotify_EnemyFinishAttack::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) { return; }

	// The owning actor of `MeshComp`.
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) { return; }

	if (AMIR_EnemyBASE* Enemy = Cast<AMIR_EnemyBASE>(OwnerActor))
	{
		Enemy->TriggerFinishAttackFromNotify();
	}
	else if (UMIR_Combat_Component* CombatComp = OwnerActor->FindComponentByClass<UMIR_Combat_Component>())
	{
		CombatComp->FinishCurrentAttack();
	}
}

FString UMIR_AnimNotify_EnemyFinishAttack::GetNotifyName_Implementation() const
{
	return TEXT("Enemy: Finish Attack");
}
