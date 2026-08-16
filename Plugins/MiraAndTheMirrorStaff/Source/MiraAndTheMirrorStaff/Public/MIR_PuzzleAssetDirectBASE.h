// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MIR_PlayerChar.h"
#include "MIR_PuzzleAssetManager.h"
#include "MIR_PuzzleAssetsIndirectBASE.h"
#include "MIR_PuzzleAssetDirectBASE.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectActivated, AMIR_PuzzleAssetManager*, DirectAsset);

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetDirectBASE : public AMIR_PuzzleAssetManager
{
	GENERATED_BODY()
	
protected:
	// Collision for direct base.
	UPROPERTY(EditAnywhere, Category = "CollisionComponent")
		USphereComponent* CollisionComponent;

	// Mesh for direct base. 
	UPROPERTY(EditAnywhere, Category = "Mesh")
		USkeletalMeshComponent* Mesh;

public:
	// Delegate object to call Activate for Direct.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Active")
		FOnDirectActivated OnDirectActivated;

	// Linked actor for indirect puzzle base.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Link")
		AMIR_PuzzleAssetsIndirectBASE* LinkedActor = nullptr;

	// Checks if child asset is turned on. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
		bool bDidTurnOn = false;

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetDirectBASE();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets Direct to activate.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Active")
		void ActivateDirect(AMIR_PlayerChar* PlayerCharacter);

	// Sets Direct to deactivate.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Deactive")
		void DeactivateDirect(AMIR_PlayerChar* PlayerCharacter);
	
};
