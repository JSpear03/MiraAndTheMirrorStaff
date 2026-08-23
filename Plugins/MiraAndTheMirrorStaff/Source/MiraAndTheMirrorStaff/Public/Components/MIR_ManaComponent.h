// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_ManaComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ManaComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Timer handle for the cooldown timer.
	FTimerHandle CooldownHandle;

public:	
	// Maximum amount of Mana Power.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana Power")
		int32 MaxManaPower = 16;

	// Represents amount of mana power shown in the viewport.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Power")
		int32 TotalManaPower = 8;

	// Current Mana Power.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Power")
		int32 ManaPower = 8;

	// Sets default values for this component's properties
	UMIR_ManaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the component ends.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Increases `ManaPower`, clamping between 0 and `TotalManaPower`.
	* 
	* @param Amount The amount by which `ManaPower` will increase.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
		void ManaRecovery(int32 Amount);

	/**
	* Decreases `ManaPower`, clamping between 0 and `TotalManaPower`.
	*
	* @param Amount The amount by which `ManaPower` will decrease.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
		void ManaReduction(int32 Amount);

	// Sets up timer for cooldown.
	// TODO: Call in `ManaReduction()` instead.
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
		void ManaCooldown();

	// Clears the cooldown timer. Must be called during actor cleanup on `EndPlay()`.
	void ClearCooldownTimer();

	/*
	* Increases `TotalManaPower`, clamping between 0 and `MaxManaPower`.
	* Then, it sets `ManaPower` to `TotalManaPower`.
	* 
	* @param BonusManaPower The amount received from the ManaUp item.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mana Power")
		void IncreaseTotalManaPower(int32 BonusManaPower);

};
