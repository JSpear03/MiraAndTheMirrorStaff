// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ArcadeFunctions.h"
#include "MIR_LevelStreamActor.generated.h"

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

	// Attemps to unload a level upon end of overlap.
	UFUNCTION()
	void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// The level to load during Level Streaming.
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> LevelToLoad;

	// A currently loaded level via Level Streaming.
	UPROPERTY(VisibleInstanceOnly)
	ULevelStreamingDynamic* LoadedLevel = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Overlap volume to trigger Level Streaming.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;
};
