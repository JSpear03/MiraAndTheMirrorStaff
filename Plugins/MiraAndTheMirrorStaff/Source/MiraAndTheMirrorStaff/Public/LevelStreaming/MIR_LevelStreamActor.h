// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ArcadeFunctions.h"
#include "MIR_LevelStreamActor.generated.h"

/**
 * The Level-Stream Actor (LSA) is an actor that streams/unsteams one assigned level
 * when the player character (MIR_PlayerChar) begins/ends an overlap with its BoxComponent
 * (`OverlapVolume`).
 * 
 * When placed in the editor, its `LevelToLoad` member should be assigned to a UMAP reference
 * so that the level can be loaded.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_LevelStreamActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	AMIR_LevelStreamActor();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	/**
	 * Attempts to stream a level upon overlap.
	 *
	 * @param OverlappedComponent The component that began the overlap.
	 * @param OtherActor The actor that began the overlap (ideally a player character).
	 * @param OtherComp The specific primitive that began the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComp` that began
	 * the overlap.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Detailed collision info if a sweep did occur.
	 */
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Attempts to unload a level upon end of overlap.
	 *
	 * @param OverlappedComponent The component that ended the overlap.
	 * @param OtherActor The actor that ended the overlap (ideally a player character).
	 * @param OtherComp The specific primitive that ended the overlap.
	 * @param OtherBodyIndex The index of the body within `OtherComp` that ended
	 * the overlap.
	 */
	UFUNCTION()
		void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// The level to load via Level Streaming.
	UPROPERTY(EditAnywhere, Category = "Streaming|Level")
		TSoftObjectPtr<UWorld> LevelToLoad;

	// A currently loaded level via Level Streaming.
	UPROPERTY(VisibleInstanceOnly, Category = "Streaming|Level")
		ULevelStreamingDynamic* LoadedLevel = nullptr;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

private:
	// Overlap volume to trigger Level Streaming.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),
			  Category = "Streaming|Volume")
		UBoxComponent* OverlapVolume;

	/**
	 * Validates whether the `OtherActor` is the first player character in the world.
	 * Should be called during overlap events.
	 * 
	 * @param OtherActor The actor which initiated/ended the overlap with the LSA.
	 */
	const bool ValidateOtherActorAsPlayer(const AActor* OtherActor) const;

	// Streams in a level as long as it is valid.
	void StreamLevel();

	// Unstreams in a level as long as it is valid.
	void UnstreamLevel();
};
