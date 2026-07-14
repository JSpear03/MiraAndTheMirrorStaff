// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_CopiableComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMIR_CopiableComponent::UMIR_CopiableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMIR_CopiableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMIR_CopiableComponent::TickComponent(float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMIR_CopiableComponent::CopyMagic(AMIR_PlayerChar* CurrentPlayer, FString MagicToCopy)
{
	// Even if the object does not contain an interface, 
	// setting a condition prevents errors or inaccuracies.
	if (UKismetSystemLibrary::DoesImplementInterface(CurrentPlayer, UMIR_Copiable::StaticClass()))
	{
		IMIR_Copiable::CopyMagic(CurrentPlayer, MagicToCopy);
	}
}
