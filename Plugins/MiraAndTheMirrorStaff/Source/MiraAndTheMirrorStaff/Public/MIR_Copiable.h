// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MIR_PlayerChar.h"
#include "MIR_Copiable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMIR_Copiable : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for the Copiable Component to check if player can copy magic. 
 */
class MIRAANDTHEMIRRORSTAFF_API IMIR_Copiable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// When called, the function stores copied magic for the player.
	virtual void CopyMagic(AMIR_PlayerChar* CurrentPlayer, FString MagicToCopy);   
};
