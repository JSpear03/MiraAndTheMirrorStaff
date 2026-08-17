// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMIR_Interactable.generated.h"

class AMIR_PlayerChar;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIMIR_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for environmental assets that respond to the player's Interact input,
 *  such as Treasure Chests and Lore Tapestries.
 */
class MIRAANDTHEMIRRORSTAFF_API IIMIR_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Called when the player uses the Interact input on this asset.
	// Each Interactable asset defines its own response (open chest, show lore, etc.).
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AMIR_PlayerChar* CurrentPlayer);
};
