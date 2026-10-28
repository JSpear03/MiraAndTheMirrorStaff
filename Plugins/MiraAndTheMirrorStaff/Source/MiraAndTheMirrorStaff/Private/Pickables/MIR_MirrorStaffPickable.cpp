// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Pickables/MIR_MirrorStaffPickable.h"
#include "Save/MIR_SaveSubsystem.h"

// Sets default values.
AMIR_MirrorStaffPickable::AMIR_MirrorStaffPickable()
{
	PrimaryActorTick.bCanEverTick = false;  
}

// Called when game starts.
void AMIR_MirrorStaffPickable::BeginPlay()
{
	Super::BeginPlay();

	if (IsActorBeingDestroyed()) { return; }

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_MirrorStaffPickable::OnMirrorBeginOverlap);
}

void AMIR_MirrorStaffPickable::OnMirrorBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// The player that overlapped with this item.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(OtherActor);

	if (!OtherActor || !PlayerCharacter) { return; }

	PlayerCharacter->GrantKeyItem(EMIR_KeyItem::MirrorStaff);

	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
		{
			SaveSubsystem->MarkPickupCollected(GetEffectivePickupID());
			SaveSubsystem->SetHasMirrorStaff(true);
		}
	}

	Destroy();
}
