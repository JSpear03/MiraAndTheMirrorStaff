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
		FString AttackTypeName = "None";

	UPROPERTY(EditAnywhere, Category = "Assigned Attack Type")
		int32 Damage = 0;

	UPROPERTY(EditAnywhere, Category = "Assigned Attack Type")
		int32 ManaConsumption = 0;
};

USTRUCT(BlueprintType)
struct FMeleeAttack : public FAttackType 
{
	GENERATED_BODY();
};

USTRUCT(BlueprintType)
struct FRangedAttack : public FAttackType
{
	GENERATED_BODY();
};

USTRUCT(BlueprintType)
struct FAOEAttack : public FAttackType
{
	GENERATED_BODY();
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_AttackTypeComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Attack")
		TArray<FAttackType> AttackTypes;

public:	
	// Attack type that is not considered an attack. 
	// Perfect for copy particles. 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		FAttackType NoneAttackType;

	// Used for attacking closely with a dagger.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		FMeleeAttack MeleeAttackType;
	
	// Attacks that involve a far away distance. Useful for staff. 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		FRangedAttack RangedAttackType;

	// An attack that hits multiple enemies and surrounds the player.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		FAOEAttack AOEAttackType;


	// Sets default values for this component's properties
	UMIR_AttackTypeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Add new attack type. 
	UFUNCTION(BlueprintCallable)
		void AddStructToAttack(FAttackType NewAttackType);

	// Returns an array of the Attack Type struct. 
	UFUNCTION(BlueprintCallable)
		TArray<FAttackType> GetAttackType();
};
