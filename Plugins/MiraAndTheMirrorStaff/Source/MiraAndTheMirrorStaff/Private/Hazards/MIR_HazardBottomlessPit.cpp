// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazards/MIR_HazardBottomlessPit.h"
#include "Components/BoxComponent.h"

// Sets default values.
AMIR_HazardBottomlessPit::AMIR_HazardBottomlessPit()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when game starts.
void AMIR_HazardBottomlessPit::BeginPlay()
{
	Super::BeginPlay();
}

void AMIR_HazardBottomlessPit::OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnHazardBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,
								bFromSweep, SweepResult);
}
