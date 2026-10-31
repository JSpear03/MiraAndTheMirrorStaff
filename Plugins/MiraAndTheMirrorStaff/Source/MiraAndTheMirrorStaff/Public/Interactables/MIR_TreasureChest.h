// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IMIR_Interactable.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "MIR_TreasureChest.generated.h"

/**
 * A treasure chest that implements the Interactable Interface.
 * When interacted with, it opens and grants the player the key item inside.
 * Once opened, it never grants its item again.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_TreasureChest : public AActor, public IIMIR_Interactable
{
	GENERATED_BODY()

public:
	// The chest's visible mesh. Also what the Interact overlap check detects.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Treasure Chest")
		class UStaticMeshComponent* ChestMesh;

	// The key item granted to the player when the chest is opened.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Treasure Chest")
		EMIR_KeyItem ContainedKeyItem = EMIR_KeyItem::MirrorStaff;

	// Sets default values for the Treasure Chest's properties.
	AMIR_TreasureChest();

	// Opens the chest and grants the player the key item inside.
	virtual void OnInteract_Implementation(AMIR_PlayerChar* CurrentPlayer) override;

protected:
	// Whether the chest has already been opened.
	UPROPERTY(BlueprintReadOnly, Category = "Treasure Chest")
		bool bOpened = false;

	// Blueprint hook for opening feedback (lid animation, sparkle particles, etc.).
	UFUNCTION(BlueprintImplementableEvent, Category = "Treasure Chest")
		void OnOpened();
};
