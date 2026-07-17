// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ElementComponent.h"

// Sets default values for this component's properties
UMIR_ElementComponent::UMIR_ElementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Default values for Flame ELement.
	FlameElement.ElementName = "Flame";
	FlameElement.ElementStrength = "Ice";
	FlameElement.ElementWeakness = "Thunder";
	FlameElement.ElementalNeutralities = {"Nothing", "Flame"};
	FlameElement.ElementProperty = "Burn";

	// Default values for Ice Element.
	IceElement.ElementName = "Ice";
	IceElement.ElementStrength = "Thunder";
	IceElement.ElementWeakness = "Flame";
	IceElement.ElementalNeutralities = { "Nothing", "Ice" };
	IceElement.ElementProperty = "Freeze";

	// Default values for Thunder Element. 
	ThunderElement.ElementName = "Thunder";
	ThunderElement.ElementStrength = "Flame";
	ThunderElement.ElementWeakness = "Ice";
	ThunderElement.ElementalNeutralities = { "Nothing", "Thunder" };
	ThunderElement.ElementProperty = "Electrocute";
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

void UMIR_ElementComponent::AddStructToElement(FElementMap NewElementType)
{
	ElementMap.Add(NewElementType);
}

TArray<FElementMap> UMIR_ElementComponent::GetElementMap()
{
	return ElementMap;
}
