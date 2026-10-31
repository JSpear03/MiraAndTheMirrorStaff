// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Hazards/MIR_HazardSpikes.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Components/BoxComponent.h"

// Sets default values.
AMIR_HazardSpikes::AMIR_HazardSpikes()
{
	// Spikes hurt only on contact; the player jumps over them rather than
	// falling in, so no respawn teleport and no enemy deletion.
	bTeleportsPlayerToRespawn = false;
	bDeletesEnemies = false;

	// Blocks movement so that the spikes must be jumped over.
	CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetNotifyRigidBodyCollision(true);

	CollisionBox->OnComponentHit.AddDynamic(this, &AMIR_HazardSpikes::OnSpikesHit);
}

void AMIR_HazardSpikes::OnSpikesHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) { return; }

	if (AMIR_PlayerChar* Player = Cast<AMIR_PlayerChar>(OtherActor))
	{
		Player->ApplyHazardDamage(Damage);

		OnPlayerContact(Hit.ImpactPoint);
	}
}
