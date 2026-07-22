// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ElementComponent.h"

// Sets default values for this component's properties
UMIR_ElementComponent::UMIR_ElementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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

TArray<FString> UMIR_ElementComponent::GetElementNames()
{
	return {"Ranged Nothing", "Nothing", "Flame", "Ice", "Thunder"};
}

TArray<FString> UMIR_ElementComponent::GetElementProperties()
{
	return {"None", "Burn", "Freeze", "Electrocute"};
}
