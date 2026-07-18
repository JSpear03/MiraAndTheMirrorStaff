// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_AttackTypeComponent.h"

// Sets default values for this component's properties
UMIR_AttackTypeComponent::UMIR_AttackTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Melee attack default values. 
	MeleeAttackType.AttackTypeName = "Melee";
	MeleeAttackType.Damage = 2;
	MeleeAttackType.ManaConsumption = 2;

	// Ranged attack default values.
	RangedAttackType.AttackTypeName = "Ranged";
	RangedAttackType.Damage = 1;
	RangedAttackType.ManaConsumption = 1;

	// Aoe attack default values
	AOEAttackType.AttackTypeName = "AOE";
	AOEAttackType.Damage = 4;
	AOEAttackType.ManaConsumption = 3;
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

void UMIR_AttackTypeComponent::AddStructToAttack(FAttackType NewAttackType)
{
	AttackTypes.Add(NewAttackType);
}

TArray<FAttackType> UMIR_AttackTypeComponent::GetAttackType()
{
	return AttackTypes;
}

