// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "EMIR_AttackTypes.h"
#include "SMIR_AttackTypeData.h"
#include "MIR_AttackTypeComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttackType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assigned Attack Type", meta = (GetOptions = "GetAttackNames"))
		FString AttackTypeName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assigned Attack Type", meta = (ClampMin = 0, ClampMax = 10, UIMin = 0, UIMax = 10))
		int32 Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assigned Attack Type", meta = (ClampMin = 0, ClampMax = 10, UIMin = 0, UIMax = 10))
		int32 ManaConsumption = 0;
};


/**
 * Specifies an Attack Type for a summonable magic.
 * This Attack Type will have associated damage and mana-consumption values to be used
 * in damage calculations, etc.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_AttackTypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets attack type data for the magic classes. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		FAttackType AttackType;

	// Sets default values for this component's properties
	UMIR_AttackTypeComponent();

	// Default Structs
	const FDefaultTypeStruct DefaultTypeStruct;  // Default struct for Attack Types.
	const FMeleeStruct MeleeStruct;				 // Default struct for Melee.
	const FRangedStruct RangedStruct;			 // Default struct for Ranged.
	const FAoEStruct AoEStruct;					 // Default struct for AoE.

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Determines working data for the specified Attack Type (via enum).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Type Details")
		FDefaultTypeStruct AttackTypeStruct = DefaultTypeStruct;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Returns the `AttackTypeName` member variable.
	UFUNCTION(BlueprintCallable, Category = "Attack Type Details", meta = (KeyWords = "Get Name", ReturnDisplayName = "Name",
			  ToolTip = "Returns the `AttackTypeName` member variable."))
		EAttackTypes GetAttackTypeName() const { return AttackTypeStruct.AttackTypeName; }

	// Returns the `Damage` member variable.
	UFUNCTION(BlueprintCallable, Category = "Attack Type Details", meta = (KeyWords = "Get Damage", ReturnDisplayName = "Damage",
			  ToolTip = "Returns the `Damage` member variable."))
		int32 GetDamage() const { return AttackTypeStruct.Damage; }

	// Returns the `ManaConsumption` member variable.
	UFUNCTION(BlueprintCallable, Category = "Attack Type Details", meta = (KeyWords = "Get Mana", ReturnDisplayName = "ManaConsumption",
			  ToolTip = "Returns the `ManaConsumption` member variable."))
		int32 GetManaConsumption() const { return AttackTypeStruct.ManaConsumption; }

	/**
	 * @brief Sets the `AttackTypeStruct` member variable based on an Enum.
	 * 
	 * The Enum parameter is passed to a `switch` statement, which checks each
	 * case to then set the `AttackTypeStruct` appropriately.
	 * 
	 * @param ENewAttackType A specifier to determine the new struct for the `AttackTypeStruct`.
	 * 
	 * @returns N/A
	 */
	void SetAttackTypeDetails(EAttackTypes ENewAttackType = EAttackTypes::None);

private:
	// Returns a list of the attack names.
	UFUNCTION()
		TArray<FString> GetAttackNames();
};
