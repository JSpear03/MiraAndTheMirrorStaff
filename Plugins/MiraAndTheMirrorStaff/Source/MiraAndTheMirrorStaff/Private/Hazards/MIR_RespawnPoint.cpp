// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazards/MIR_RespawnPoint.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Components/BoxComponent.h"

// Sets default values.
AMIR_RespawnPoint::AMIR_RespawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMIR_RespawnPoint::OnTriggerBeginOverlap);
}

void AMIR_RespawnPoint::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMIR_PlayerChar* Player = Cast<AMIR_PlayerChar>(OtherActor);

	// Marks this point as where the player will reappear after falling into a hazard.
	if (Player)
	{
		Player->SetLastRespawnPoint(this);
	}
}
