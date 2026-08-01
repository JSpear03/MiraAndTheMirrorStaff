// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PuzzleAssetsIndirectBASE.h"

// Sets default values
AMIR_PuzzleAssetsIndirectBASE::AMIR_PuzzleAssetsIndirectBASE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetsIndirectBASE::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMIR_PuzzleAssetsIndirectBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMIR_PuzzleAssetsIndirectBASE::ActivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Executed turn on for Indirect."));

	bDidTurnOn = true;
}

void AMIR_PuzzleAssetsIndirectBASE::DeactivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Executed turn off for Indirect."));

	bDidTurnOn = false;
}

