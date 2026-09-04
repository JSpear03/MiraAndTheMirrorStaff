// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/MIR_LoreTapestry.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values.
AMIR_LoreTapestry::AMIR_LoreTapestry()
{
	PrimaryActorTick.bCanEverTick = false;

	TapestryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapestryMesh"));
	TapestryMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = TapestryMesh;
}

void AMIR_LoreTapestry::OnInteract_Implementation(AMIR_PlayerChar* CurrentPlayer)
{
	OnLoreOpened();
}
