// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_ElementComponent.generated.h"

USTRUCT(BlueprintType) 
struct FElementMap
{
	GENERATED_BODY()

	// Name of the element. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
	FString ElementName;

	// List of element strengths for the name. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
	TArray<FString> ElementStrengths;

	// List of element weaknesses for the name. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
	TArray<FString> ElementWeaknesses;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ElementComponent : public UActorComponent
{
	GENERATED_BODY()

	// List of elements. 
	UPROPERTY(EditAnywhere, Category = "Elements")
		TArray<FElementMap> ElementMap;

public:	
	// Sets default values for this component's properties
	UMIR_ElementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Adds an element to the map. 
	UFUNCTION(BlueprintCallable)
		void SetElementMap(FString Name, TArray<FString> Strengths, TArray<FString> Weaknesses);

	// Gets the current element map. 
	UFUNCTION(BlueprintCallable)
		TArray<FElementMap> GetElementMap();

	// Returns the calculated damage.
	UFUNCTION(BlueprintCallable, Category = "Elements")
		int32 CalculateDamage();
};
