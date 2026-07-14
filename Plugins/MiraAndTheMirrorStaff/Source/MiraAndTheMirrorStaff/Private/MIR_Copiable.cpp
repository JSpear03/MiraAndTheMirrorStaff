// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_Copiable.h"

// Add default functionality here for any IMIR_Copiable functions that are not pure virtual.

void IMIR_Copiable::CopyMagic(AMIR_PlayerChar* CurrentPlayer, FString MagicToCopy)
{
	CurrentPlayer->SetStoredMagic(MagicToCopy);
}
