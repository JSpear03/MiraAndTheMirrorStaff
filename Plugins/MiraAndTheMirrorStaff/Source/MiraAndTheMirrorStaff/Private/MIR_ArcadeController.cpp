// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_ArcadeController.h"
#include "EnhancedInputComponent.h"
#include "IMIR_Interactable.h"
#include "MIR_PlayerChar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called to setup input actions.
void AMIR_ArcadeController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	ETriggerEvent ETriggered, EStarted, ECompleted;

	ETriggered = ETriggerEvent::Triggered;
	EStarted = ETriggerEvent::Started;
	ECompleted = ETriggerEvent::Completed;

	if (EnhancedInput)
	{
		// Triggered Bindings.
		EnhancedInput->BindAction(Move, ETriggered, this, &AMIR_ArcadeController::MoveCharacter);
		EnhancedInput->BindAction(MainAttack, ETriggered, this, &AMIR_ArcadeController::UseMainAttack);
		EnhancedInput->BindAction(RangedCopy, ETriggered, this, &AMIR_ArcadeController::UseMirrorStaff);
		EnhancedInput->BindAction(SubAttack, ETriggered, this, &AMIR_ArcadeController::UseSubAttack);
		EnhancedInput->BindAction(Discard, ETriggered, this, &AMIR_ArcadeController::DiscardCopiedMagic);
		EnhancedInput->BindAction(RangedCopy, ETriggered, this, &AMIR_ArcadeController::UseMirrorStaff);
		
		// Started Bindings.
		EnhancedInput->BindAction(Interact, EStarted, this, &AMIR_ArcadeController::UseInteract);
		EnhancedInput->BindAction(PauseAction, EStarted, this, &AMIR_ArcadeController::PauseMenu);
		EnhancedInput->BindAction(MapAction, EStarted, this, &AMIR_ArcadeController::OpenMap);

		// Jump Bindings.
		EnhancedInput->BindAction(Jump, EStarted, this, &AMIR_ArcadeController::StartPlayerJumping);
		EnhancedInput->BindAction(Jump, ECompleted, this, &AMIR_ArcadeController::StopPlayerJumping);

		// Dash Bindings.
		EnhancedInput->BindAction(Dash, EStarted, this, &AMIR_ArcadeController::StartDashing);
		EnhancedInput->BindAction(Dash, ECompleted, this, &AMIR_ArcadeController::StopDashing);
	}
}

void AMIR_ArcadeController::BeginPlay()
{
	Super::BeginPlay();
}

/************************************************************************************
 * MOBILITY ACTIONS — GENERAL MOVEMENT
 ***********************************************************************************/

void AMIR_ArcadeController::MoveCharacter(const FInputActionValue& ActionValue)
{
	/**
	 * // ===== OLD MOVEMENT CODE ====
	 * 
	 * //ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	 * //FRotator ControllerRotation = PlayerCharacter->GetControlRotation();
	 * //FVector ActionVector, ForwardVector, RightVector;
	 *
	 * //ActionVector = ActionValue.Get<FVector>();
	 *
	 * //ControllerRotation = UKismetMathLibrary::MakeRotFromX(ActionVector);
	 *
	 * //PlayerCharacter->SetActorRotation(ControllerRotation, ETeleportType::None);
	 *
	 * //ForwardVector = UKismetMathLibrary::GetForwardVector(ControllerRotation);
	 * //RightVector = UKismetMathLibrary::GetRightVector(ControllerRotation);
	 *
	 * //PlayerCharacter->AddMovementInput(ForwardVector, ActionVector.Y);  // Movement on the Y-Axis.
	 * //PlayerCharacter->AddMovementInput(RightVector, ActionVector.X);    // Movement on the X-Axis.
	 */

	// Get player-character reference and necessary vectors and rotaters.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);  // Player-character reference.
	FRotator ControllerRotation = PlayerCharacter->GetControlRotation();  // Player character's control rotation.
	FVector ActionVector, ForwardVector, RightVector;					  // Movement- and rotation-related vector.

	ActionVector = ActionValue.Get<FVector>();
	ForwardVector = UKismetMathLibrary::GetForwardVector(ControllerRotation);  // Forward vector off control rotation.
	RightVector = UKismetMathLibrary::GetRightVector(ControllerRotation);      // Right vector off control rotation.

	// Add movement to the player character.
	PlayerCharacter->AddMovementInput(ForwardVector, ActionVector.Y);  // Movement on the Y-Axis.
	PlayerCharacter->AddMovementInput(RightVector, ActionVector.X);    // Movement on the X-Axis.

}

/************************************************************************************
 * MOBILITY ACTIONS — JUMPING
 ***********************************************************************************/

void AMIR_ArcadeController::StartPlayerJumping()
{
	// Check whether the player has the item to jump.
	if (!IsPlayerJumping())
	{
		// If the condition results in True (returns False -> negated to True), do nothing.
		return;
	}
	else
	{
		// If the condition results in False (returns True -> negated to False), start jumping.
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->Jump();
	}
}

void AMIR_ArcadeController::StopPlayerJumping()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->StopJumping();
}

bool AMIR_ArcadeController::IsPlayerJumping() const
{
	// A reference to the player character.
	AMIR_PlayerChar* PlayerCharacter =
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerCharacter)
	{
		// If the reference is invalid, return false.
		return false;
	}
	else
	{
		// If the reference is valid, return the value in this Boolean.
		return PlayerCharacter->bHasSkyboundShoes;
	}
}

/************************************************************************************
 * MOBILITY ACTIONS — DASHING
 ***********************************************************************************/

void AMIR_ArcadeController::StartDashing()
{
	// Check whether the player has the item to dash.
	if (!IsPlayerDashing())
	{
		// If the condition results in True (returns False -> negated to True), do nothing.
		return;
	}
	else
	{
		// If the condition results in False (returns True -> negated to False), start dashing.
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();

		CharacterMovement->MaxWalkSpeed = 900;
	}
}

void AMIR_ArcadeController::StopDashing()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();

	CharacterMovement->MaxWalkSpeed = 600;
}

bool AMIR_ArcadeController::IsPlayerDashing() const
{
	// A reference to the player character.
	AMIR_PlayerChar* PlayerCharacter =
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerCharacter)
	{
		// If the reference is invalid, return false.
		return false;
	}
	else
	{
		// If the reference is valid, return the value in this Boolean.
		return PlayerCharacter->bHasSpiritStole;
	}
}

/************************************************************************************
 * COMBAT ACTIONS — MAIN ATTACK
 ***********************************************************************************/

void AMIR_ArcadeController::AllowMainAttack()
{
	bCanUseMainAttack = true;
}

/************************************************************************************
 * COMBAT ACTIONS — SUB-ATTACK
 ***********************************************************************************/

void AMIR_ArcadeController::UseSubAttack()
{
	GEngine->AddOnScreenDebugMessage(3, 5, FColor::Blue, TEXT("Use Dagger."));
}

/************************************************************************************
 * OTHER ACTIONS — INTERACTION
 ***********************************************************************************/

void AMIR_ArcadeController::UseInteract()
{
	AMIR_PlayerChar* PlayerCharacter =
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerCharacter)
	{
		return;
	}

	// Object types that Interactable assets can be (static and movable props).
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(PlayerCharacter);

	// Finds all actors within the Interact range of the player.
	TArray<AActor*> NearbyActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerCharacter->GetActorLocation(),
		InteractRange, ObjectTypes, AActor::StaticClass(), IgnoredActors, NearbyActors);

	AActor* ClosestInteractable = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (AActor* NearbyActor : NearbyActors)
	{
		// Only assets that implement the Interactable Interface can respond.
		if (NearbyActor->Implements<UIMIR_Interactable>())
		{
			float Distance = PlayerCharacter->GetDistanceTo(NearbyActor);

			if (Distance < ClosestDistance)
			{
				ClosestInteractable = NearbyActor;
				ClosestDistance = Distance;
			}
		}
	}

	if (ClosestInteractable)
	{
		IIMIR_Interactable::Execute_OnInteract(ClosestInteractable, PlayerCharacter);
	}
}
