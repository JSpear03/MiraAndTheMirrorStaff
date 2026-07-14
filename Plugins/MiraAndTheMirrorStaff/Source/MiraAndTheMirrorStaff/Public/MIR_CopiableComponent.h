// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_Copiable.h"
#include "MIR_CopiableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_CopiableComponent : public UActorComponent, public IMIR_Copiable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMIR_CopiableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	* Calls interface to store magic for the player.
	* 
	* The Get Player function should be casted to the Player Character class, 
	* in order for the Copy Magic function to pass in the player object. 
	*/
	UFUNCTION(BlueprintCallable, Category = "Copy Magic")
	virtual void CopyMagic(AMIR_PlayerChar* CurrentPlayer, FString MagicToCopy) override;
};
