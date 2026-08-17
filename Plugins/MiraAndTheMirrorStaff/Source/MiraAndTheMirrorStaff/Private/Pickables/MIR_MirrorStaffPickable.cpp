// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickables/MIR_MirrorStaffPickable.h"

// Sets default values.
AMIR_MirrorStaffPickable::AMIR_MirrorStaffPickable()
{
	PrimaryActorTick.bCanEverTick = false;  
}

// Called when game starts.
void AMIR_MirrorStaffPickable::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_MirrorStaffPickable::OnMirrorBeginOverlap);
}

void AMIR_MirrorStaffPickable::OnMirrorBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
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

	PlayerCharacter->bHasMirrorStaff = true;

	Destroy();
}
