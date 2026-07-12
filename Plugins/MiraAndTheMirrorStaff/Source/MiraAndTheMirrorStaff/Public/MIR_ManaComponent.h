// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_ManaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ManaComponent : public UActorComponent
{
	GENERATED_BODY()

	// Maximum amount of Mana Power.
	UPROPERTY(EditAnywhere, Category = "Mana Power")
	int32 MaxManaPower = 100;

	// Current Mana Power.
	UPROPERTY(EditAnywhere, Category = "Mana Power")
	int32 ManaPower;  

public:	
	// Sets default values for this component's properties
	UMIR_ManaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Increases mana power. 
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
	void ManaRecovery(int32 Amount);

	// Decreases mana power.
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
	void ManaReduction(int32 Amount);

	// When Mana Power reaches 0, it recovers all mana slowly. 
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
	void ManaCooldown() const;
};
