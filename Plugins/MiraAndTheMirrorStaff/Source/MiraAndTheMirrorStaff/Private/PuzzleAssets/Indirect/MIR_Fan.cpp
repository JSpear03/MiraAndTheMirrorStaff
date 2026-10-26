// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "PuzzleAssets/Indirect/MIR_Fan.h"
#include "Kismet/GameplayStatics.h"

AMIR_Fan::AMIR_Fan()
{
	SkeletonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeleton Mesh");

	SkeletonMesh->SetupAttachment(RootComponent);

	// Saves performance by turning off can use tick, since it is unecessary for the fan.
	PrimaryActorTick.bCanEverTick = false;
}
