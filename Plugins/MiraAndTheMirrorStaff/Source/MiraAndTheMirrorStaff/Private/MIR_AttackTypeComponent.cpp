// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_AttackTypeComponent.h"

// Sets default values for this component's properties
UMIR_AttackTypeComponent::UMIR_AttackTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMIR_AttackTypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UMIR_AttackTypeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FString> UMIR_AttackTypeComponent::GetAttackNames()
{
	return {"None", "Melee", "Ranged", "AOE"};
}

