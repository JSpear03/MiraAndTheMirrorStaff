// Fill out your copyright notice in the Description page of Project Settings.

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
 * 
 * The optional member `bStreamImmediately` can be used to stream in a level immediately.
 * However, this will disable overlap events, preventing the level from being unstreamed
 * through said events.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_LevelStreamActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMIR_LevelStreamActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Attempts to stream a level upon overlap.
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	// Attempts to unload a level upon end of overlap.
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
	// Called every frame
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
	const bool ValidateOtherActorAsPlayer(const AActor* OtherActor);

	// Streams in a level as long as it is valid.
	void StreamLevel();

	// Unstreams in a level as long as it is valid.
	void UnstreamLevel();
};
