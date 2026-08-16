// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ManaUp.h"

// Sets default values.
AMIR_ManaUp::AMIR_ManaUp()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when game begins.
void AMIR_ManaUp::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_ManaUp::OnManaOverlap);
}

void AMIR_ManaUp::OnManaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);
	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	if (!OtherActor || !bIsChildOfPlayer)
	{
		return;
	}

	PlayerCharacter->ManaComponent->IncreaseTotalManaPower(ManaBonus);

	Destroy();
}
