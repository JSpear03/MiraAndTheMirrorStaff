// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IMIR_Interactable.h"
#include "Components/BoxComponent.h"
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

	// Collision for popping up a widget whether the player would overlap with the box or not.
	UPROPERTY(EditAnywhere, Category = "Interact")
		UBoxComponent* InteractCollision;

public:
	// The tapestry's visible mesh. Also what the Interact overlap check detects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore Tapestry")
		class UStaticMeshComponent* TapestryMesh;

	// Sets default values for the Lore Tapestry's properties.
	AMIR_LoreTapestry();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Displays this tapestry's lore text and enables Game and UI input.
	virtual void OnInteract_Implementation(AMIR_PlayerChar* CurrentPlayer) override;

protected:
	// Whether this tapestry's text box is currently on screen.
	// Prevents a second box from opening while one is displayed.
	bool bLoreOpen = false;

	// Blueprint hook that displays the large lore text box widget.
	UFUNCTION(BlueprintImplementableEvent, Category = "Lore Tapestry")
		void OnLoreOpened();

	/*
	* When it begins overlapping with the interactable collision.
	*
	* @param OtherActor is represented by the player.
	*/
	UFUNCTION()
		void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	/*
	* When something exits from the interactable collision.
	*
	* @param OtherActor is represented by the player.
	*/
	UFUNCTION()
		void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:
	// Adds the interact UI to the viewport. 
	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
		void OpenInteractUI();

	// Removes the interact UI from the viewport.
	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
		void CloseInteractUI();
};
