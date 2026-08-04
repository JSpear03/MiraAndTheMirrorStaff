// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMIR_AttackTypes.h"
#include "SMIR_AttackTypeData.h"
#include "MIR_AttackTypeComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttackType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assigned Attack Type", meta = (GetOptions = "GetAttackNames"))
		FString AttackTypeName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assigned Attack Type")
		int32 Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assigned Attack Type")
		int32 ManaConsumption = 0;
};


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Sets Attack Type information for a magic class.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Type|Name")
		EAttackTypes EThisAttackType = EAttackTypes::Melee;

	// Determines working data for the specified Attack Type (via enum).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Type|Data")
		FDefaultTypeStruct AttackTypeStruct;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Returns a list of the attack names.
	UFUNCTION()
		TArray<FString> GetAttackNames();
};
