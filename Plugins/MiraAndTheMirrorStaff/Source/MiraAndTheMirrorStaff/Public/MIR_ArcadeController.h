// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcadeController.h"
#include "MIR_ArcadeController.generated.h"

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_ArcadeController : public AArcadeController
{
	GENERATED_BODY()
	
	UPROPERTY()
	UInputAction* MoveAction;

protected:
	// Called to setup input actions. 
	virtual void SetupInputComponent() override;

	// Called to move character.
	void MoveCharacter(const FInputActionValue& ActionValue);
};
