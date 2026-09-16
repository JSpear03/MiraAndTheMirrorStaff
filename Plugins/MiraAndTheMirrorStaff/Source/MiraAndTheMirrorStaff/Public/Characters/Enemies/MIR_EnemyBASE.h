// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/MIR_CharacterBASE_M.h"
#include "Interfaces/IMIR_Copiable.h"
#include "Components/MIR_CopiableComponent.h"
#include "MIR_EnemyBASE.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, AMIR_EnemyBASE*, SelfRef);

/**
 *  Base class for all enemies in the game. 
 *  Includes logic such as spawning dust, as well as checking if enemy's magic can be copied.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_EnemyBASE : public AMIR_CharacterBASE_M, public IIMIR_Copiable
{
	GENERATED_BODY()

	// Prevents tick from repeating broadcast calls. 
	UPROPERTY()
		bool bCanRunBroadcast = true;
	
public:
	// Sets default value damage when player contacts enemy. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Contact Damage")
		int32 ContactDamage = 1;

	// Implements copy ability. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copy")
		UMIR_CopiableComponent* CopiableComponent;

	// Handles AI combat attack states and trigger events.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class UMIR_Combat_Component* CombatComponent;

	// Delegate to call function when enemy's health is 0.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "State")
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

	// Interface entry point. Any system that wants this enemy's magic (the Mirror Staff's ranged
	// copy, for example) calls IIMIR_Copiable::Execute_CopyMagic on the enemy Actor, and the
	// enemy hands the work to its Copiable Component.
	virtual void CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer,
		TSubclassOf<class AMIR_MagicBASE> MagicToCopy) override;

	/**
	* Called when enemy delgate is broadcasted.
	* 
	* @param SelfRef An optional reference to self.
	*/
	UFUNCTION(BlueprintImplementableEvent)
		void EnemyDied(AMIR_EnemyBASE* SelfRef);

	// Change the material to a certain color, after dust is setup. 
	UFUNCTION(BlueprintImplementableEvent)
		void SetNewMaterial();

private:
	// Checks if enemy's health reaches 0.
	void CheckHealth();

};
