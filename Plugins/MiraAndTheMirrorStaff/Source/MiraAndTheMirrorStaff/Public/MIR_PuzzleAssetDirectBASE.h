// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MIR_PlayerChar.h"
#include "IMIR_Indirect.h"
#include "MIR_PuzzleAssetsIndirectBASE.h"
#include "MIR_PuzzleAssetDirectBASE.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetDirectBASE : public AActor, public IIMIR_Indirect
{
	GENERATED_BODY()
	
	// Collision for direct base.
	UPROPERTY(EditAnywhere, Category = "CollisionComponent")
		USphereComponent* CollisionComponent;

	// Mesh for direct base. 
	UPROPERTY(EditAnywhere, Category = "Mesh")
		USkeletalMeshComponent* Mesh;

	// Checks if child asset is turned on. 
	UPROPERTY(EditAnywhere, Category = "Button")
		bool bDidTurnOn = false;

	// Linked actor for indirect puzzle base.
	UPROPERTY(EditAnywhere, Category = "Link")
		AMIR_PuzzleAssetsIndirectBASE* LinkedActor = nullptr;
public:	
	// Sets default values for this actor's properties
	AMIR_PuzzleAssetDirectBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when player collides 
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets Indirect to activate.
	virtual void ActivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter) override;

	// Sets Indirect to deactivate. 
	virtual void DeactivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter) override;

};
