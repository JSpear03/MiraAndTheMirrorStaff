// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ElementComponent.h"

// Sets default values for this component's properties
UMIR_ElementComponent::UMIR_ElementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMIR_ElementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UMIR_ElementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMIR_ElementComponent::SetElementMap(FString Name, FColor Color, FString Symbol, 
	FString Strength, FString Weakness, TArray<FString> Neutrals, FString Property)
{
	FElementMap NewElementMap;

	NewElementMap.ElementName = Name;
	NewElementMap.MainColor = Color;
	NewElementMap.ElementSymbol = Symbol;
	NewElementMap.ElementStrength = Strength;
	NewElementMap.ElementWeakness = Weakness;
	NewElementMap.ElementalNeutralities = Neutrals;
	NewElementMap.ElementProperty = Property;

	ElementMap.Add(NewElementMap);
}

TArray<FElementMap> UMIR_ElementComponent::GetElementMap()
{
	return ElementMap;
}

