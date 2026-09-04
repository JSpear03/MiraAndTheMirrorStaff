// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MIR_CharacterBASE_M.h"

// Sets default values
AMIR_CharacterBASE_M::AMIR_CharacterBASE_M()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates Health Component
	HealthComponent = CreateDefaultSubobject<UMIR_HealthComponent>(TEXT("HealthComponent"));
	
	// Creates Element Component and assigns default Element.
	ElementComponent = CreateDefaultSubobject<UMIR_ElementComponent>(TEXT("ElementComponent"));
	ElementComponent->SetCurrentElement(EElements::Nothing);
}

// Called when the game starts or when spawned
void AMIR_CharacterBASE_M::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMIR_CharacterBASE_M::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMIR_CharacterBASE_M::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/************************************************************************************
 * SUSTAINING DAMAGE
 ***********************************************************************************/

void AMIR_CharacterBASE_M::CalculateDamage(EElements AttackerElement, EElements RecieverElement,
	int32 AttackerDamageValue)
{
	// The damage multiplier based on Elemental weakness and strength.
	// By default, zero represents what the `AttackerElement` is weak against.
	int32 Modifier = 0;

	if (AttackerElement == EElements::Nothing || RecieverElement == EElements::Nothing  // Netural Against
		|| AttackerElement == RecieverElement)
	{
		Modifier = 1;
	}
	else if ((AttackerElement == EElements::Flame   && RecieverElement == EElements::Ice) ||  // Strong against
			 (AttackerElement == EElements::Ice     && RecieverElement == EElements::Thunder) ||
			 (AttackerElement == EElements::Thunder && RecieverElement == EElements::Flame))
	{
		Modifier = 2;
	}

	HealthComponent->HealthReduction(Modifier * AttackerDamageValue);
}

/************************************************************************************
 * GETTERS & SETTERS
 ***********************************************************************************/

void AMIR_CharacterBASE_M::SetStoredMagic(TSubclassOf<class AMIR_MagicBASE> MagicType)
{
	StoredMagicType = MagicType;
}

TSubclassOf<class AMIR_MagicBASE> AMIR_CharacterBASE_M::GetStoredMagic() const
{
	return StoredMagicType;
}
