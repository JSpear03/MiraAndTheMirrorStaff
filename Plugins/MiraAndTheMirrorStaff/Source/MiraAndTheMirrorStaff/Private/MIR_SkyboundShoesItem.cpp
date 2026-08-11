// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_SkyboundShoesItem.h"
#include "MIR_PlayerChar.h"

// Sets default values.
AMIR_SkyboundShoesItem::AMIR_SkyboundShoesItem()
{
}

// Called when game starts.
void AMIR_SkyboundShoesItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, 
		&AMIR_SkyboundShoesItem::OnSkyboundShoesOverlap);
}

void AMIR_SkyboundShoesItem::OnSkyboundShoesOverlap(UPrimitiveComponent* OverlappedComponent, 
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

	PlayerCharacter->GrantKeyItem(EMIR_KeyItem::SkyboundShoes);

	Destroy();
}
