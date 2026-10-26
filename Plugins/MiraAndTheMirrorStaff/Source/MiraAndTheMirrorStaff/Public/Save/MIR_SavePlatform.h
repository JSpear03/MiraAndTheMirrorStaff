// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "MIR_SavePlatform.generated.h"

/**
 * A platform that opens the Save Screen upon overlap with a player.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_SavePlatform : public AActor
{
	GENERATED_BODY()

	// Collision for the Save Platform.
	UPROPERTY(EditAnywhere, Category = "Collision")
		UBoxComponent* BoxCollision;

	// Messh for Save Platform.
	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* PlatformMesh;
	
public:	
	// Sets default values for this actor's properties
	AMIR_SavePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	* @brief Called upon an actor overlapping with the Save Platform.
	* 
	* Validates A) the `OtherActor` and the currently possessed player character,
	* then B) whether said actor is a child of `AMIR_PlayerChar`.
	* 
	* If both succeed, then `CallSaveRoomMenu()`--which creates that Widget--is called.
	* 
	* @param 
	* - OtherActor: The actor that overlaps with the platform,
	* and is represented by the Player.
	*/
	UFUNCTION()
		void OnSaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Adds Save Room UI to the viewport. 
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
		void CallSaveRoomMenu(AMIR_PlayerChar* PlayerRef);
};
