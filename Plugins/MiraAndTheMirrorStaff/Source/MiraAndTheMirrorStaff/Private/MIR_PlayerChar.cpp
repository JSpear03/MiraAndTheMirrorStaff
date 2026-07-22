// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PlayerChar.h"

// Sets default values.
AMIR_PlayerChar::AMIR_PlayerChar()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	ManaComponent = CreateDefaultSubobject<UMIR_ManaComponent>(TEXT("Mana Component"));

	SpringArm->SetupAttachment(RootComponent); 

	// Attaches camera to the end of the spring arm. 
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 

	// Allows the player's yaw to match the camera yaw. 
	// Needs to be turned off, since the point of view might be third person.
	bUseControllerRotationYaw = false; 

	SpringArm->bUsePawnControlRotation = true;  // Allows rotation of the spring arm. 

	ElementComponent->ElementType.ElementName = "Ranged Nothing";
} 

void AMIR_PlayerChar::SetStoredMagic(FString MagicType)
{
	StoredMagicType = MagicType;
}

FString AMIR_PlayerChar::GetStoredMagic()
{
	return StoredMagicType;
}
