// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleAssets/MIR_PuzzleAssetBASE.h"
#include "MIR_PuzzleAssetsIndirectBASE.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIndirectLinked);

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetsIndirectBASE : public AMIR_PuzzleAssetBASE
{
	GENERATED_BODY()

public:	
	//// Sets indirect to activate.
	//UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category = "Indirect")
	//	FOnIndirectLinked OnIndirectLinked;

	//// Checks if child asset is turned on. 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	//	bool bDidTurnOn = false;

	//// Puzzle state to activate and deactivate a puzzle action.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle State")
	//	EPuzzleState PuzzleState = EPuzzleState::Deactivate;

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetsIndirectBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// Sets Indirect to activate.
	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Active")
	//	void ActivateIndirect();

	//// Sets Indirect to deactivate.
	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Deactive")
	//	void DeactivateIndirect();

	//// Broadcasts the Activate function from the OnIndirectLinked delegate.
	//UFUNCTION(BlueprintCallable, Category = "Active")
	//	void CallActivateDelegate();
};
