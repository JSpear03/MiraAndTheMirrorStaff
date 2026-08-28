// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Player/MIR_PlayerChar.h"  // TODO: Is this necessary?
#include "PuzzleAssets/MIR_PuzzleAssetBASE.h"
#include "Enums/EMIR_PuzzleState.h"
#include "Components/MIR_ElementComponent.h"
#include "MIR_PuzzleAssetDirectBASE.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectActivated,   AMIR_PuzzleAssetDirectBASE*, SelfRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectDeactivated, AMIR_PuzzleAssetDirectBASE*, SelfRef);

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetDirectBASE : public AMIR_PuzzleAssetBASE
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//// Collision for direct base.
	//UPROPERTY(EditAnywhere, Category = "CollisionComponent")
	//	USphereComponent* CollisionComponent;

	//// Mesh for direct base. 
	//UPROPERTY(EditAnywhere, Category = "Mesh")
	//	USkeletalMeshComponent* Mesh;

	//// Puzzle state to activate and deactivate a puzzle action.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle State")
	//	EPuzzleState PuzzleState = EPuzzleState::Deactivate;

	// The Element Component, which dictates the actor's interactions with magic.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
		class UMIR_ElementComponent* ElementComponent;

	/**
	* When something overlaps with the DirectBASE actor.
	*
	* @param OtherActor The actor that overlapped with this actor's `CollisionComponent`.
	*/
	UFUNCTION(BlueprintCallable)
	void HandleOverlap(AActor* OtherActor);

	// When the `CapsuleCollision` begins overlapping with something.
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

public:
	// Delegate object to call Activate for Direct.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "State")
		FOnDirectActivated OnDirectActivated;

	// Delegate object to call Deactivate for Direct.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "State")
		FOnDirectDeactivated OnDirectDeactivated;

	//// Linked actor for indirect puzzle base.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Link")
	//	AMIR_PuzzleAssetsIndirectBASE* LinkedActor = nullptr;

	//// Checks if child asset is turned on. 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	//	bool bDidTurnOn = false;

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetDirectBASE();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// Sets Direct to activate.
	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Active")
	//	void ActivateDirect();

	//// Sets Direct to deactivate.
	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Deactive")
	//	void DeactivateDirect();
	
};
