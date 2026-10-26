// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Hazards/MIR_HazardBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Components/BoxComponent.h"

// Sets default values.
AMIR_HazardBASE::AMIR_HazardBASE()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = CollisionBox;
}

// Called when game starts.
void AMIR_HazardBASE::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMIR_HazardBASE::OnHazardBeginOverlap);
}

void AMIR_HazardBASE::OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleActorContact(OtherActor);
}

void AMIR_HazardBASE::HandleActorContact(AActor* OtherActor)
{
	AMIR_PlayerChar* Player = Cast<AMIR_PlayerChar>(OtherActor);

	if (Player)
	{
		// Captured before the respawn teleport so feedback plays where the player fell in.
		const FVector ImpactPoint = Player->GetActorLocation();

		Player->ApplyHazardDamage(Damage);

		if (bTeleportsPlayerToRespawn)
		{
			Player->RespawnAtLastPoint();
		}

		OnPlayerContact(ImpactPoint);
	}
	else if (bDeletesEnemies && Cast<AMIR_EnemyBASE>(OtherActor))
	{
		OtherActor->Destroy();
	}
}
