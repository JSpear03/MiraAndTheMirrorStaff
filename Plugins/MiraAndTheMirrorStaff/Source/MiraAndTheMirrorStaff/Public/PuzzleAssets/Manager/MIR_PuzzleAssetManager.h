// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MIR_PuzzleAssetManager.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Array of all the direct assets in the level. 
	// For now, it will just be a TArray to the parent manager, 
	// so there would not be potential errors in cirular dependencies.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direct")
		TArray<AMIR_PuzzleAssetManager*> DirectContainer;

	// Array of all the indirect puzzle assets in level. 
	// May use it at a later time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indirect")
		TArray<AMIR_PuzzleAssetManager*> IndirectAssets;

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetManager();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* Sets current DirectBASE to valid.
	* 
	* @param DirectAsset A DirectBASE object referenced as the parent manager, thereby
	* preventing circular dependency. It would then be converted to the DirectBASE class.
	*/
	UFUNCTION(BlueprintCallable, Category = "Direct")
		void FindDirectBase(AMIR_PuzzleAssetManager* DirectAsset);

	/*
	* Checks whether a DirectBASE object is on.
	* 
	* @param DirectAsset A pointer to a DirectBASE object, upon which the function will check
	* for active by checking its `bDidTurnOn` boolean.
	*/
	UFUNCTION(BlueprintCallable, Category = "Direct")
		void CheckActiveInDirect(AMIR_PuzzleAssetManager* DirectAsset);

	// Checks if every element in the `DirectContainer` is on.
	UFUNCTION(BlueprintCallable, Category = "Direct")
		int32 FindAllDirectActive();

	/*
	* Starts timer to set either all DirectBASE objects to Active or Deactive.
	* 
	* @param Duration The amount of time until the Deactivation of all assets.
	*/
	UFUNCTION(BlueprintCallable, Category = "Timer")
		void SetActiveTimer(float Duration);
	
	/**
	 * Called during the timer to check whether all DirectBASE objects are on.
	 * If so, their linked IndirectBASE is activated.
	 */
	UFUNCTION(BlueprintCallable, Category = "Timer")
		void DuringDuration();
};
