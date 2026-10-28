// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Pickables/MIR_ManaUp.h"
#include "Save/MIR_SaveSubsystem.h"

// Sets default values.
AMIR_ManaUp::AMIR_ManaUp()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when game begins.
void AMIR_ManaUp::BeginPlay()
{
	Super::BeginPlay();

	if (IsActorBeingDestroyed()) { return; }

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_ManaUp::OnManaOverlap);
}

void AMIR_ManaUp::OnManaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// The player that overlapped with this item.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(OtherActor);

	if (!OtherActor || !PlayerCharacter) { return; }

	PlayerCharacter->ManaComponent->IncreaseTotalManaPower(ManaBonus);

	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
		{
			SaveSubsystem->MarkPickupCollected(GetEffectivePickupID());
		}
	}

	Destroy();
}
