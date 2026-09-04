// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazards/MIR_HazardWater.h"
#include "Magic/MIR_MagicBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values.
AMIR_HazardWater::AMIR_HazardWater()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


// Called when the game starts or when spawned.
void AMIR_HazardWater::BeginPlay()
{
	Super::BeginPlay();
}

void AMIR_HazardWater::OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnHazardBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,
		bFromSweep, SweepResult);

	AMIR_MagicBASE* Magic = Cast<AMIR_MagicBASE>(OtherActor);

	// Only the dedicated ice hitbox of an Ice-element spell can freeze water.
	// The element check matters because every spell currently spawns an
	// IceCollisionComponent regardless of its element.
	if (Magic && OtherComp == Magic->IceCollisionComponent &&
		Magic->ElementComponent->GetElementName() == EElements::Ice)
	{
		OnFreezeHit(Magic->GetActorLocation());
	}

	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(OtherActor);

	// Attempts to load game over if player is found. It would actually switch once health reaches 0.
	//if (PlayerCharacter)
	//{
	//	PlayerCharacter->LoadingGameOver();
	//}
}
