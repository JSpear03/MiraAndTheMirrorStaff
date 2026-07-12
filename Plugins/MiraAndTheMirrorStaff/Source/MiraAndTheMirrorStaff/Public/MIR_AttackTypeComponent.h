// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_AttackTypeComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttackType 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Assigned Attack Type")
		FString AttackTypeName;

	UPROPERTY(EditAnywhere, Category = "Assigned Attack Type")
		int32 Damage;

	UPROPERTY(EditAnywhere, Category = "Assigned Attack Type")
		int32 ManaConsumption;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_AttackTypeComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Attack")
		TArray<FAttackType> AttackTypes;

public:	
	// Sets default values for this component's properties
	UMIR_AttackTypeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Sets and adds new attack type to the Attack Types array. 
	void SetAttackType(FString NewName, int32 NewDamage, int32 NewManaConsumption);

	// Returns an array of the Attack Type struct. 
	TArray<FAttackType> GetAttackType();
};
