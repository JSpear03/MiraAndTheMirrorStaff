// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PlayerChar.h"

// Sets default values.
AMIR_PlayerChar::AMIR_PlayerChar()
{
	ManaComponent = CreateDefaultSubobject<UMIR_ManaComponent>(TEXT("Mana Component"));
}

void AMIR_PlayerChar::SetStoredMagic(FString MagicType)
{
	StoredMagicType = MagicType;
}

FString AMIR_PlayerChar::GetStoredMagic()
{
	return StoredMagicType;
}
