// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_EnemyBASE.h"

// Sets default values.
AMIR_EnemyBASE::AMIR_EnemyBASE()
{
	CopiableComponent = CreateDefaultSubobject<UMIR_CopiableComponent>(TEXT("Copiable Component"));

	ElementComponent->ElementType.ElementalNeutralities = { "Nothing", "Flame", "Ice", "Thunder" };
}

void AMIR_EnemyBASE::BeginPlay()
{
	Super::BeginPlay();

	OnEnemyDied.AddDynamic(this, &AMIR_EnemyBASE::EnemyDied);
}

// Called every frame.
void AMIR_EnemyBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComponent->GetHealth() == 0 && bCanRunBroadcast)
	{
		bCanRunBroadcast = false;
		
		OnEnemyDied.Broadcast();
	}
}
