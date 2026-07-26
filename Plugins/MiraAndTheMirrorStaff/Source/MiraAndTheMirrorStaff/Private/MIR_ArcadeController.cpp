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

// TODO: Comment out the interface once we have the Interactable interface.
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

void AMIR_ArcadeController::StartPlayerJumping()
{
	if (!IsPlayerJumping())
	{
		return;
	}

	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->Jump();
}

void AMIR_ArcadeController::StopPlayerJumping()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->StopJumping();
}

void AMIR_ArcadeController::StartDashing()
{
	if (!IsPlayerDashing())
	{
		return;
	}

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
