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

void UMIR_ElementComponent::SetElementMap(FString Name, TArray<FString> Strengths, TArray<FString> Weaknesses)
{
	FElementMap NewMap;

	NewMap.ElementName = Name;
	NewMap.ElementStrengths = Strengths;
	NewMap.ElementWeaknesses = Weaknesses;

	ElementMap.Add(NewMap);
}

TArray<FElementMap> UMIR_ElementComponent::GetElementMap()
{
	return ElementMap;
}

// TODO: Add calculation for damage. 
int32 UMIR_ElementComponent::CalculateDamage()
{
	return int32();
}

