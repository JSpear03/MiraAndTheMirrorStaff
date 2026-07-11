// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMIR_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===== VARIABLES =====

	// Represents the owner's health, where every two units is equivalent to one heart.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 Health = 10;
	
	// ===== FUNCTIONS =====

	/**
	* Gets the owner's `Health` variable.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		int32 GetHealth();

	/**
	* Sets the owner's `Health` variable.
	* The result will be clamped between 0 (min) and 100 (max).
	* 
	* @param NewHealth The value to be assigned to the `Health` variable.
	* Subject to clamping after being passed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void SetHealth(int32 NewHealth);
};
