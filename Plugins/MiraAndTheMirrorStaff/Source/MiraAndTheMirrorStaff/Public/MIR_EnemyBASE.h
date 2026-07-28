// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_CharacterBASE_M.h"
#include "MIR_CopiableComponent.h"
#include "MIR_EnemyBASE.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDied);

/**
 *  Base class for all enemies in the game. 
 *  Includes logic such as spawning dust, as well as checking if enemy's magic can be copied.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_EnemyBASE : public AMIR_CharacterBASE_M
{
	GENERATED_BODY()

	// Prevents tick from repeating broadcast calls. 
	UPROPERTY()
		bool bCanRunBroadcast = true;
	
public:
	// Sets default value damage when player contacts enemy. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Contact Damage")
		int32 ContactDamage = 1;

	// Implements copy ability. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copy")
		UMIR_CopiableComponent* CopiableComponent;

	// Delegate to call function when enemy's health is 0.
	UPROPERTY()
		FOnEnemyDied OnEnemyDied;

	// Sets default values for Base Enemy. 
	AMIR_EnemyBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
			const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when enemy delgate is broadcasted.
	UFUNCTION(BlueprintImplementableEvent)
		void EnemyDied();
};
