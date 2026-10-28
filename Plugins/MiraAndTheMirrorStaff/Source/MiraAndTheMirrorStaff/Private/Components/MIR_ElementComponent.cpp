// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Components/MIR_ElementComponent.h"

// Sets default values for this component's properties
UMIR_ElementComponent::UMIR_ElementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMIR_ElementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Configure Element details:
	if (CurrentElement != EElements::None)
	{
		SetElementDetails(CurrentElement);
	}
	else if (ElementStruct.ElementName != EElements::None)
	{
		CurrentElement = ElementStruct.ElementName;
		SetElementDetails(CurrentElement);
	}
	else if (AActor* MyOwner = GetOwner())
	{
		const FString OwnerName = MyOwner->GetClass()->GetName();
		if (OwnerName.Contains(TEXT("Flame"), ESearchCase::IgnoreCase) ||
			OwnerName.Contains(TEXT("Fire"),  ESearchCase::IgnoreCase))
		{
			SetElementDetails(EElements::Flame);
		}
		else if (OwnerName.Contains(TEXT("Ice"),   ESearchCase::IgnoreCase) ||
				 OwnerName.Contains(TEXT("Frost"), ESearchCase::IgnoreCase))
		{
			SetElementDetails(EElements::Ice);
		}
		else if (OwnerName.Contains(TEXT("Thunder"),   ESearchCase::IgnoreCase) ||
				 OwnerName.Contains(TEXT("Lightning"), ESearchCase::IgnoreCase))
		{
			SetElementDetails(EElements::Thunder);
		}
	}
}

// Called every frame
void UMIR_ElementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMIR_ElementComponent::SetElementDetails(EElements ENewElement)
{
	CurrentElement = ENewElement;

	switch (ENewElement)
	{
	case EElements::None:  // Configure Component for None
	{
		FDefaultElementStruct DefaultElementStruct;  // Default struct for None.
		ElementStruct = DefaultElementStruct;
		break;
	}
	case EElements::Nothing:  // Configure Component for Nothing
	{
		FNothingStruct NothingStruct;  // Default struct for Nothing.
		ElementStruct = NothingStruct;
		break;
	}
	case EElements::Flame: // Configure Component for Flame
	{
		FFlameStruct FlameStruct;  // Default struct for Flame.
		ElementStruct = FlameStruct;
		break;
	}
	case EElements::Ice:    // Configure Component for Ice
	{
		FIceStruct IceStruct;  // Default struct for Ice.
		ElementStruct = IceStruct;
		break;
	}
	case EElements::Thunder:    // Configure Component for Thunder
	{
		FThunderStruct ThunderStruct;  // Default struct for Thunder.
		ElementStruct = ThunderStruct;
		break;
	}
	default:
		break;  // Do nothing because FDefaultStruct is already defined.
	}
}

void UMIR_ElementComponent::SetCurrentElement(EElements ENewElement)
{
	CurrentElement = ENewElement;
}
