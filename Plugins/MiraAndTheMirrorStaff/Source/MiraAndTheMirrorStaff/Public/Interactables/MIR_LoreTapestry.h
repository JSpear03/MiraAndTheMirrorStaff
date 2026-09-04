// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IMIR_Interactable.h"
#include "MIR_LoreTapestry.generated.h"

/**
 * A structure that implements the Interactable Interface.
 * When interacted with, it opens a large text box with lore about the
 * dungeon and its former inhabitants, switching input to Game and UI
 * so the text box can be clicked while gameplay continues.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_LoreTapestry : public AActor, public IIMIR_Interactable
{
	GENERATED_BODY()

public:
	// The tapestry's visible mesh. Also what the Interact overlap check detects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore Tapestry")
		class UStaticMeshComponent* TapestryMesh;

	// The lore text displayed when the player interacts with this tapestry.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore Tapestry",
	//	meta = (MultiLine = "true"))
	//	FText LoreText;

	// Sets default values for the Lore Tapestry's properties.
	AMIR_LoreTapestry();

	// Displays this tapestry's lore text and enables Game and UI input.
	virtual void OnInteract_Implementation(AMIR_PlayerChar* CurrentPlayer) override;

protected:
	// Whether this tapestry's text box is currently on screen.
	// Prevents a second box from opening while one is displayed.
	bool bLoreOpen = false;

	// Blueprint hook that displays the large lore text box widget.
	UFUNCTION(BlueprintImplementableEvent, Category = "Lore Tapestry")
		void OnLoreOpened();
};
