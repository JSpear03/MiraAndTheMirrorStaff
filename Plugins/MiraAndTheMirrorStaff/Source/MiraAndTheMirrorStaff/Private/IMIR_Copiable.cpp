// Fill out your copyright notice in the Description page of Project Settings.


#include "IMIR_Copiable.h"

// Add default functionality here for any IMIR_Copiable functions that are not pure virtual.

void IIMIR_Copiable::CopyMagic(AMIR_PlayerChar* CurrentPlayer, FString MagicToCopy)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Called from Interface."));

	CurrentPlayer->SetStoredMagic(MagicToCopy);
}
