// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ArcadeController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called to setup input actions.
void AMIR_ArcadeController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	ETriggerEvent Triggered, Started, Completed;

	Triggered = ETriggerEvent::Triggered;
	Started = ETriggerEvent::Started;
	Completed = ETriggerEvent::Completed;

	if (EnhancedInput)
	{
		// Triggered Bindings.
		EnhancedInput->BindAction(Move, Triggered, this, &AMIR_ArcadeController::MoveCharacter);
		EnhancedInput->BindAction(MainAttack, Triggered, this, &AMIR_ArcadeController::UseMainAttack);
		EnhancedInput->BindAction(RangedCopy, Triggered, this, &AMIR_ArcadeController::UseMirrorStaff);
		EnhancedInput->BindAction(SubAttack, Triggered, this, &AMIR_ArcadeController::UseSubAttack);
		EnhancedInput->BindAction(Discard, Triggered, this, &AMIR_ArcadeController::DiscardCopiedMagic);
		EnhancedInput->BindAction(RangedCopy, Triggered, this, &AMIR_ArcadeController::UseMirrorStaff);
		
		// Started Bindings.
		EnhancedInput->BindAction(Interact, Started, this, &AMIR_ArcadeController::UseInteract);
		EnhancedInput->BindAction(PauseAction, Started, this, &AMIR_ArcadeController::PauseMenu);
		EnhancedInput->BindAction(MapAction, Started, this, &AMIR_ArcadeController::OpenMap);

		// Jump Bindings.
		EnhancedInput->BindAction(Jump, Started, this, &AMIR_ArcadeController::StartPlayerJumping);
		EnhancedInput->BindAction(Jump, Completed, this, &AMIR_ArcadeController::StopPlayerJumping);

		// Dash Bindings.
		EnhancedInput->BindAction(Dash, Started, this, &AMIR_ArcadeController::StartDashing);
		EnhancedInput->BindAction(Dash, Completed, this, &AMIR_ArcadeController::StopDashing);
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

void AMIR_ArcadeController::UseMainAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::FromHex("#08FFA7FF"), TEXT("Attack With Staff"));
}

void AMIR_ArcadeController::UseMirrorStaff()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::FromHex("#0879FFFF"), 
		TEXT("Use Mirror Staff for copying."));
}

void AMIR_ArcadeController::UseSubAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::FromHex("#63626DFF"), 
		TEXT("Attack with Dagger."));
}

void AMIR_ArcadeController::DiscardCopiedMagic()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::FromHex("#FF401AFF"), 
		TEXT("Discard Copied Magic"));
}

void AMIR_ArcadeController::UseInteract()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::FromHex("#00D4FFFF"), 
		TEXT("Press [F] to Interact and pay respects."));
}

void AMIR_ArcadeController::StartPlayerJumping()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->Jump();
}

void AMIR_ArcadeController::StopPlayerJumping()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->StopJumping();
}

void AMIR_ArcadeController::StartDashing()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();

	CharacterMovement->MaxWalkSpeed = 900;
}

void AMIR_ArcadeController::StopDashing()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();

	CharacterMovement->MaxWalkSpeed = 600;
}
