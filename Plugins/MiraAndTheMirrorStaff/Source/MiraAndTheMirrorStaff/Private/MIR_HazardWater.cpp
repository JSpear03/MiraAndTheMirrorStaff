// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_HazardWater.h"
#include "MIR_MagicBASE.h"
#include "MIR_ElementComponent.h"
#include "Components/CapsuleComponent.h"

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
}
