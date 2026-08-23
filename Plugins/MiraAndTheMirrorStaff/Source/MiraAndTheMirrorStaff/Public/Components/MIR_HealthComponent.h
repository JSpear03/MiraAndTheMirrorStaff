// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

	// Represents the owner's health, where every two units is equivalent to one heart.
	UPROPERTY(EditAnywhere, Category = "Stats")
		int32 Health = 8;

public:	
	// Represents the maximum amount of health shown on screen. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 TotalHealth = 8;

	// Maximum amount of health. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 MaxHealth = 16;

	// Sets default values for this component's properties
	UMIR_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	* Gets the owner's `Health` variable.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stats", meta = (KeyWords = "Get",
			  ReturnDisplayName = "Health"))
		int32 GetHealth() const;

	/**
	* Sets the owner's `Health` variable.
	* The result will be clamped between 0 (min) and 100 (max).
	* 
	* @param NewHealth The value to be assigned to the `Health` variable.
	* Subject to clamping after being passed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void SetHealth(int32 NewHealth);


	// Increases curent Health up to the Total Health. 
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void HealthRecovery(int32 Amount);

	// Decreases Current Health. 
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void HealthReduction(int32 Amount);

	/*
	* Increase the amount of hearts in viewport.
	* 
	* @param BonusHealth is the amount recieved from the HealthUp item.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void IncreaseTotalHealth(int32 BonusHealth);
};
