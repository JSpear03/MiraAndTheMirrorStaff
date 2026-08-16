// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_SpiritStoleItem.h"

// Sets default values. 
AMIR_SpiritStoleItem::AMIR_SpiritStoleItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when game starts.
void AMIR_SpiritStoleItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, 
		&AMIR_SpiritStoleItem::OnSpiritStoleOverlap);
}

void AMIR_SpiritStoleItem::OnSpiritStoleOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);
	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	if (!OtherActor || !bIsChildOfPlayer)
	{
		return;
	}

	PlayerCharacter->GrantKeyItem(EMIR_KeyItem::SpiritStole);

	Destroy();
}
