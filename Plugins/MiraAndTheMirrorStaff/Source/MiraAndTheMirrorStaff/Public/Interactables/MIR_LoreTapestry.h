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
 * so that the text box can be clicked while gameplay continues.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_LoreTapestry : public AActor, public IIMIR_Interactable
{
	GENERATED_BODY()

	// Collision for displaying a widget upon overlap with a player character.
	UPROPERTY(EditAnywhere, Category = "Interact")
		UBoxComponent* InteractCollision;

public:
	// The tapestry's visible mesh. Also what the Interact's overlap check detects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore Tapestry")
		class UStaticMeshComponent* TapestryMesh;

	// Sets default values for the Lore Tapestry's properties.
	AMIR_LoreTapestry();

protected:
	// Called when the game starts or when spawned.
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

	/**
	 * Called when an actor overlaps with this actor's collision area.
	 * 
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComponent The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Called when an actor ends its overlap with this actor's collision area.
	 *
	 * @param OverlappedComponent The component that ended the overlap.
	 * @param OtherActor The actor that ended the overlap (ideally a player character).
	 * @param OtherComponent The specific primitive that ended the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComponent` that ended
	 * the overlap.
	 */
	UFUNCTION()
		void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:
	// Adds the interact UI to the viewport. 
	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
		void OpenInteractUI();

	// Removes the interact UI from the viewport.
	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
		void CloseInteractUI();
};
