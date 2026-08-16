// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_HealthUp.h"

// Sets default values
AMIR_HealthUp::AMIR_HealthUp()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AMIR_HealthUp::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_HealthUp::OnHealthOverlap);
}

void AMIR_HealthUp::OnHealthOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

	PlayerCharacter->HealthComponent->IncreaseTotalHealth(HealthBonus);

	Destroy();
}

// Called every frame
void AMIR_HealthUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

