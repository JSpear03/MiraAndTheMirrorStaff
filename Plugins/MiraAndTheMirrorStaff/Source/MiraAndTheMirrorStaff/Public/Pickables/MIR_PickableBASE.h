// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "MIR_PickableBASE.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PickableBASE : public AActor
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties
	AMIR_PickableBASE();

protected:
	// Collision for any child of PickableBASE.
	UPROPERTY(EditAnywhere, Category = "CollisionComponent")
		USphereComponent* CollisionComponent;

	// Mesh for any child of PickableBASE. 
	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* Mesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
