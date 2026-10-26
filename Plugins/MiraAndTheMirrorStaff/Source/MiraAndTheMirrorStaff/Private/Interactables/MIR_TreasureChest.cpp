// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Interactables/MIR_TreasureChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values.
AMIR_TreasureChest::AMIR_TreasureChest()
{
	PrimaryActorTick.bCanEverTick = false;

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = ChestMesh;
}

void AMIR_TreasureChest::OnInteract_Implementation(AMIR_PlayerChar* CurrentPlayer)
{
	// A chest's item can be claimed only once.
	if (bOpened || !CurrentPlayer)
	{
		return;
	}

	bOpened = true;

	CurrentPlayer->GrantKeyItem(ContainedKeyItem);

	OnOpened();
}
