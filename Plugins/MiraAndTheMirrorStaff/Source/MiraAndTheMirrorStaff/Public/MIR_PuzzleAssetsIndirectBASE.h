// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IMIR_Indirect.h"
#include "MIR_PlayerChar.h"
#include "MIR_PuzzleAssetsIndirectBASE.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetsIndirectBASE : public AActor, public IIMIR_Indirect
{
	GENERATED_BODY()
	
	// Checks if child asset is turned on. 
	UPROPERTY(EditAnywhere, Category = "Button")
		bool bDidTurnOn = false;

public:	
	// Sets default values for this actor's properties
	AMIR_PuzzleAssetsIndirectBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets Indirect to activate.
	virtual void ActivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter) override;

	// Sets Indirect to deactivate. 
	virtual void DeactivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter) override;

};
