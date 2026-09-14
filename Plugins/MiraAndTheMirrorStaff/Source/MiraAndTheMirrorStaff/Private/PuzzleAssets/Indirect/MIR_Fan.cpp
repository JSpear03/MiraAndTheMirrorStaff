// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAssets/Indirect/MIR_Fan.h"
#include "Kismet/GameplayStatics.h"

AMIR_Fan::AMIR_Fan()
{
	SkeletonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeleton Mesh");

	SkeletonMesh->SetupAttachment(RootComponent);

	// Saves performance by turning off can use tick, since it is unecessary for the fan.
	PrimaryActorTick.bCanEverTick = false;
}
