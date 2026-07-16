// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ArcadeController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

// Called to setup input actions.
void AMIR_ArcadeController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent)
	{
		return;
	}

	if (MoveAction != nullptr)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMIR_ArcadeController::MoveCharacter);
	}
}

void AMIR_ArcadeController::MoveCharacter(const FInputActionValue& ActionValue)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FRotator ControllerRotation = PlayerCharacter->GetControlRotation();
	FVector ActionVector, ForwardVector, RightVector;

	ActionVector = ActionValue.Get<FVector>();
	ForwardVector = UKismetMathLibrary::GetForwardVector(ControllerRotation);
	RightVector = UKismetMathLibrary::GetRightVector(ControllerRotation);

	PlayerCharacter->AddMovementInput(ForwardVector, ActionVector.Y);  // Movement on the Y Axis.
	PlayerCharacter->AddMovementInput(RightVector, ActionVector.X);  // Movement on the X Axis.
}
