// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickables/MIR_KeyGemstone.h"

// Sets default values.
AMIR_KeyGemstone::AMIR_KeyGemstone()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when game starts.
void AMIR_KeyGemstone::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this,
		&AMIR_KeyGemstone::OnGemstoneOverlap);
}

void AMIR_KeyGemstone::OnGemstoneOverlap(UPrimitiveComponent* OverlappedComponent, 
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

	PlayerCharacter->GrantKeyItem(EMIR_KeyItem::KeyGemstone);  // Increase Gem counter.

	CheckGemColor(PlayerCharacter);
	
	Destroy();
}
