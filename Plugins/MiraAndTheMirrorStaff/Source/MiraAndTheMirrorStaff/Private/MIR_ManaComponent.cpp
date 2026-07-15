// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ManaComponent.h"

// Sets default values for this component's properties
UMIR_ManaComponent::UMIR_ManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMIR_ManaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMIR_ManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMIR_ManaComponent::ManaRecovery(int32 Amount)
{
	ManaPower = FMath::Clamp(ManaPower + Amount, 0, MaxManaPower);
}

void UMIR_ManaComponent::ManaReduction(int32 Amount)
{
	ManaPower = FMath::Clamp(ManaPower - Amount, 0, MaxManaPower);
}

void UMIR_ManaComponent::ManaCooldown() const
{
	if (ManaPower > 0)
	{
		return;
	}

	// TODO: Find a solution to Set timer error in output. 

	// Checks if world is set to prevent crashes.
	/*if (GetWorld())
	{
		FTimerHandle CooldownTimerHandler;

		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandler, this, 
			&UMIR_ManaComponent::CooldownRecovery, 1, true, 2);
	}*/
}

void UMIR_ManaComponent::CooldownRecovery()
{
	if (ManaPower != MaxManaPower)
	{
		ManaRecovery(10);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}


