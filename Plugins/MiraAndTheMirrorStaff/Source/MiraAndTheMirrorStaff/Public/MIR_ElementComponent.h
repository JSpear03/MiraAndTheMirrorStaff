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

	// Sets the color of the element.
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FColor MainColor;

	// What represents the element. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementSymbol;

	// What the current element is strong against (x2 multiplier). 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementStrength;

	// What the current element is weak against (x0 multiplier). 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementWeakness;

	// List of elements that the element name is neutral against. (x1 multiplier)
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		TArray<FString> ElementalNeutralities;

	// The type of action the element does. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementProperty;

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

	// Sets a new element and adds it to the map. 
	UFUNCTION(BlueprintCallable)
		void SetElementMap(FString Name, FColor Color, FString Symbol, FString Strength, 
			FString Weakness, TArray<FString> Neutrals, FString Property);

	// Gets the current element map. 
	UFUNCTION(BlueprintCallable)
		TArray<FElementMap> GetElementMap();
};
