// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MIR_PlayerChar.h"
#include "MIR_PuzzleAssetsIndirectBASE.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetsIndirectBASE : public AActor
{
	GENERATED_BODY()

public:	
	// Checks if child asset is turned on. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
		bool bDidTurnOn = false;

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetsIndirectBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets Indirect to activate.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Active")
		void ActivateIndirect();

	// Sets Indirect to deactivate.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Deactive")
		void DeactivateIndirect();

};
