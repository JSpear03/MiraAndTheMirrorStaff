// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MIR_PuzzleAssetDirectBASE.h"
#include "IMIR_Button.h"
#include "MIR_OnOffButton.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_OnOffButton : public AMIR_PuzzleAssetDirectBASE
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMIR_OnOffButton();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
