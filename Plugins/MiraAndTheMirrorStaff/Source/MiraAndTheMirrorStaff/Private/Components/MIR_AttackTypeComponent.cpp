// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MIR_AttackTypeComponent.h"

// Sets default values for this component's properties
UMIR_AttackTypeComponent::UMIR_AttackTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetAttackTypeDetails(CurrentAttackType, CurrentAttackSubtype);
}

void UMIR_AttackTypeComponent::PostLoad()
{
	Super::PostLoad();
	SetAttackTypeDetails(CurrentAttackType, CurrentAttackSubtype);
}

// Called when the game starts
void UMIR_AttackTypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Configure Attack Type details.
	SetAttackTypeDetails(CurrentAttackType, CurrentAttackSubtype);
}

// Called every frame
void UMIR_AttackTypeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMIR_AttackTypeComponent::SetAttackTypeDetails(EAttackTypes ENewAttackType, EAttackSubtypes ESubtype)
{
	// Default struct for Attack Types.
	// It's static because this struct is guaranteed to be constructed.
	static FDefaultTypeStruct DefaultTypeStruct;

	// Set up AttackTypeStruct.
	switch (ESubtype)
	{
	case EAttackSubtypes::Copy:    // Configure Component for Copy
		switch (ENewAttackType)
		{
		case EAttackTypes::Melee:
		{
			FMeleeCopyStruct MeleeCopyStruct;   // Default struct for Melee Copy (special type).
			AttackTypeStruct = MeleeCopyStruct;
			break;
		}
		case EAttackTypes::Ranged:
		{
			FRangedCopyStruct RangedCopyStruct;  // Default struct for Ranged Copy (special type).
			AttackTypeStruct = RangedCopyStruct;
			break;
		}
		default:
			AttackTypeStruct = DefaultTypeStruct;
			break;
		}
		break;

	case EAttackSubtypes::Dagger:  // Configure Component for Dagger
	{
		FDaggerStruct DaggerStruct;  // Default struct for Dagger (Melee subtype).
		AttackTypeStruct = DaggerStruct;
		break;
	}
		
	default:
		switch (ENewAttackType)
		{
		case EAttackTypes::None:   // Configure Component for None
			AttackTypeStruct = DefaultTypeStruct;
			break;

		case EAttackTypes::Melee:  // Configure Component for Melee
		{
			FMeleeStruct MeleeStruct;	// Default struct for Melee.
			AttackTypeStruct = MeleeStruct;
			break;
		}
		case EAttackTypes::Ranged: // Configure Component for Ranged
		{
			FRangedStruct RangedStruct;	 // Default struct for Ranged.
			AttackTypeStruct = RangedStruct;
			break;
		}
		case EAttackTypes::AoE:    // Configure Component for AoE
		{
			FAoEStruct AoEStruct;	// Default struct for AoE.
			AttackTypeStruct = AoEStruct;
			break;
		}
		default:
			break;  // Do nothing because FDefaultStruct is already defined.
		}
		break;
	}
}

void UMIR_AttackTypeComponent::SetCurrentAttackType(EAttackTypes ENewAttackType)
{
	CurrentAttackType = ENewAttackType;
	SetAttackTypeDetails(CurrentAttackType, CurrentAttackSubtype);
}
