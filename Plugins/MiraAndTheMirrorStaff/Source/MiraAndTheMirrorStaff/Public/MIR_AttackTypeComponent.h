// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Returns a list of the attack names.
	UFUNCTION()
		TArray<FString> GetAttackNames();
};
