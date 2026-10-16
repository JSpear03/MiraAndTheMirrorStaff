// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickables/MIR_SpiritStoleItem.h"
#include "Save/MIR_SaveSubsystem.h"

// Sets default values. 
AMIR_SpiritStoleItem::AMIR_SpiritStoleItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when game starts.
void AMIR_SpiritStoleItem::BeginPlay()
{
	Super::BeginPlay();

	if (IsActorBeingDestroyed())
	{
		return;
	}

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

	if (!OtherActor || !bIsChildOfPlayer || !PlayerCharacter)
	{
		return;
	}

	DisplaySpiritStole();

	PlayerCharacter->GrantKeyItem(EMIR_KeyItem::SpiritStole);

	if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMIR_SaveSubsystem>())
		{
			SaveSubsystem->MarkPickupCollected(GetEffectivePickupID());
			SaveSubsystem->SetHasSpiritStole(true);
		}
	}

	Destroy();
}
