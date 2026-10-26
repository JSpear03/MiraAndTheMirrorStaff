// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Characters/Enemies/AnimNotifies/MIR_AnimNotify_EnemyMagicSpawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Components/MIR_Combat_Component.h"

UMIR_AnimNotify_EnemyMagicSpawn::UMIR_AnimNotify_EnemyMagicSpawn()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 128, 0, 255);
#endif
}

void UMIR_AnimNotify_EnemyMagicSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	if (AMIR_EnemyBASE* Enemy = Cast<AMIR_EnemyBASE>(OwnerActor))
	{
		Enemy->TriggerMagicSpawnFromNotify();
	}
	else if (UMIR_Combat_Component* CombatComp = OwnerActor->FindComponentByClass<UMIR_Combat_Component>())
	{
		CombatComp->TriggerMagicSpawnFromNotify();
	}
}

FString UMIR_AnimNotify_EnemyMagicSpawn::GetNotifyName_Implementation() const
{
	return TEXT("Enemy: Spawn Magic");
}
