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
	if (bLoreOpen)
	{
		return;
	}

	bLoreOpen = true;

	// Game and UI input lets the player click the text box while gameplay continues.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetShowMouseCursor(true);
	}

	OnLoreOpened();
}

void AMIR_LoreTapestry::CloseLoreText()
{
	bLoreOpen = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
	}
}
