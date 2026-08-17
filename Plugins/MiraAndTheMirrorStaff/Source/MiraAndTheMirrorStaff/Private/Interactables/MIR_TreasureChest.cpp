// Fill out your copyright notice in the Description page of Project Settings.


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
