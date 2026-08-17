// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MIR_CopiableComponent.h"
#include "Characters/Player/MIR_PlayerChar.h"

// Sets default values for this component's properties
UMIR_CopiableComponent::UMIR_CopiableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

void UMIR_CopiableComponent::CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer,
	TSubclassOf<class AMIR_MagicBASE> MagicToCopy)
{
	// Reachable from Blueprint through Execute_CopyMagic, so the player cannot be assumed valid.
	if (!CurrentPlayer)
	{
		return;
	}
	
	// Debug print statement for confirmation (TODO: Delete)
	/*FString NameOfMagic = StaticEnum<EElements>()->GetValueAsString(MagicToCopy->ElementComponent->GetElementName());
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Purple, NameOfMagic);*/

	// Set the new magic.
	CurrentPlayer->SetStoredMagic(MagicToCopy);
}

