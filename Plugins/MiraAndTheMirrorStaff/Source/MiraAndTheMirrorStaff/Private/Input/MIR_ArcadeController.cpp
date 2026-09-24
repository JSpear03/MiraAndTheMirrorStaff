// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MIR_ArcadeController.h"
#include "EnhancedInputComponent.h"
#include "Interfaces/IMIR_Interactable.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Components/MIR_ManaComponent.h"
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
		// Triggered Bindings (allows holding down to continually attack at cooldown rate).
		EnhancedInput->BindAction(Move, ETriggered, this, &AMIR_ArcadeController::MoveCharacter);
		EnhancedInput->BindAction(MainAttack, ETriggered, this,
			&AMIR_ArcadeController::HandleMainAttackInput);
		EnhancedInput->BindAction(RangedCopy, ETriggered, this,
			&AMIR_ArcadeController::HandleRangedCopyInput);
		EnhancedInput->BindAction(SubAttack, ETriggered, this,
			&AMIR_ArcadeController::HandleSubAttackInput);
		EnhancedInput->BindAction(Discard, EStarted, this,
			&AMIR_ArcadeController::DiscardCopiedMagic);

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

void AMIR_ArcadeController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SubAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

/************************************************************************************
 * MOBILITY ACTIONS � GENERAL MOVEMENT
 ***********************************************************************************/

void AMIR_ArcadeController::MoveCharacter(const FInputActionValue& ActionValue)
{
	if (!bCanMove)
	{
		return;
	}

	// Get player-character reference and necessary vectors and rotaters.
	ACharacter* PlayerCharacter = 
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);  // Player-character reference.
	FRotator ControllerRotation = 
		PlayerCharacter->GetControlRotation();  // Player character's control rotation.
	FVector ActionVector, ForwardVector, RightVector;  // Movement- and rotation-related vector.

	ActionVector = ActionValue.Get<FVector>();
	ForwardVector = UKismetMathLibrary::
		GetForwardVector(ControllerRotation);  // Forward vector off control rotation.
	RightVector = 
		UKismetMathLibrary::GetRightVector(ControllerRotation);  // Right vector off control rotation.

	// Add movement to the player character.
	PlayerCharacter->AddMovementInput(ForwardVector, ActionVector.Y);  // Movement on the Y-Axis.
	PlayerCharacter->AddMovementInput(RightVector, ActionVector.X);    // Movement on the X-Axis.
}

/************************************************************************************
 * MOBILITY ACTIONS � JUMPING
 ***********************************************************************************/

void AMIR_ArcadeController::StartPlayerJumping()
{
	// Check whether the player has the item to jump.
	if (!IsPlayerJumping() || !bCanMove)
	{
		// If the condition results in True (returns False -> negated to True), do nothing.
		return;
	}
	else
	{
		// If the condition results in False (returns True -> negated to False), start jumping.
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->Jump();

		bCanAnimateJump = true;

		FTimerHandle CooldownTimerHandle;
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		const FTimerDelegate& CooldownTimerDelegate = FTimerDelegate::CreateLambda([this, &TimerManager, 
			&CooldownTimerHandle]
			{
				TimerManager.ClearTimer(CooldownTimerHandle);

				bCanAnimateJump = false;
			}
		);

		// Sets the timer, so jump animation can play properly. 
		TimerManager.SetTimer(CooldownTimerHandle, CooldownTimerDelegate, 0.5, false);
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
 * MOBILITY ACTIONS � DASHING
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
 * COMBAT ACTIONS — ATTACK INPUTS
 ***********************************************************************************/

void AMIR_ArcadeController::HandleMainAttackInput()
{
	// Both MainAttack and RangedCopy rely on bCanUseMainAttack.
	if (!bCanUseMainAttack)
	{
		return;
	}

	// Block Main Attack when the Mana Gauge is in cooldown.
	AMIR_PlayerChar* PlayerCharacter = 
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter && PlayerCharacter->ManaComponent && 
		PlayerCharacter->ManaComponent->IsCooldownActive())
	{
		return;
	}

	// Make Sub-Attack temporarily unusable for a brief window after Main Attack,
	// but unlock it before the main spell despawns so they can be paired.
	bCanUseSubAttack = false;
	if (UWorld* World = GetWorld())
	{
		LastMainAttackTime = World->GetTimeSeconds();
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
		World->GetTimerManager().SetTimer(SubAttackUnlockTimerHandle, this, 
			&AMIR_ArcadeController::UnlockSubAttack, SubAttackDelayAfterMain, false);
	}

	UseMainAttack();
	bCanUseMainAttack = false;
}

void AMIR_ArcadeController::HandleRangedCopyInput()
{
	// Both MainAttack and RangedCopy rely on bCanUseMainAttack.
	if (!bCanUseMainAttack)
	{
		return;
	}

	// Block Ranged Copy when the Mana Gauge is in cooldown
	// and if the player doesn't have the Mirror Staff.
	AMIR_PlayerChar* PlayerCharacter = 
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter && PlayerCharacter->ManaComponent && 
		PlayerCharacter->ManaComponent->IsCooldownActive())
	{
		return;
	}
	else if (PlayerCharacter && !PlayerCharacter->bHasMirrorStaff)
	{
		return;
	}

	// Make Sub-Attack temporarily unusable for a brief window, then unlock it.
	bCanUseSubAttack = false;
	if (UWorld* World = GetWorld())
	{
		LastMainAttackTime = World->GetTimeSeconds();
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
		World->GetTimerManager().SetTimer(SubAttackUnlockTimerHandle, this, 
			&AMIR_ArcadeController::UnlockSubAttack, SubAttackDelayAfterMain, false);
	}

	UseMirrorStaff();
	bCanUseMainAttack = false;
}

void AMIR_ArcadeController::HandleSubAttackInput()
{
	// Sub-Attack relies on bCanUseSubAttack.
	// It does NOT consume mana and remains usable during Mana Gauge cooldown.
	if (!bCanUseSubAttack)
	{
		return;
	}
	
	UseSubAttack();
	bCanUseSubAttack = false;
	bCanUseMainAttack = false;

	if (UWorld* World = GetWorld())
	{
		LastSubAttackTime = World->GetTimeSeconds();
		World->GetTimerManager().ClearTimer(SubAttackUnlockTimerHandle);
		World->GetTimerManager().SetTimer(SubAttackUnlockTimerHandle, this,
			&AMIR_ArcadeController::UnlockSubAttack, MinSubAttackCooldown, false);
	}
}

void AMIR_ArcadeController::UnlockSubAttack()
{
	bCanUseSubAttack = true;
	bCanUseMainAttack = true;
	SubAttackUnlockTimerHandle.Invalidate();
}

void AMIR_ArcadeController::AllowMainAttack()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		ExecuteAllowMainAttack();
		return;
	}

	const float TimeSinceAttack = World->GetTimeSeconds() - LastMainAttackTime;
	const float RemainingCooldown = MinMainAttackCooldown - TimeSinceAttack;

	if (RemainingCooldown > 0.0f)
	{
		// Minimum cooldown floor has not yet elapsed; schedule unlock when it finishes
		World->GetTimerManager().SetTimer(MainAttackUnlockTimerHandle, this,
			&AMIR_ArcadeController::ExecuteAllowMainAttack, RemainingCooldown, false);
	}
	else
	{
		ExecuteAllowMainAttack();
	}
}

void AMIR_ArcadeController::ExecuteAllowMainAttack()
{
	bCanUseMainAttack = true;

	if (UWorld* World = GetWorld())
	{
		// Only re-enable sub-attack if it is not currently on its own cooldown timer
		if (!World->GetTimerManager().IsTimerActive(SubAttackUnlockTimerHandle))
		{
			bCanUseSubAttack = true;
		}
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
	}
	else
	{
		bCanUseSubAttack = true;
	}
}

/************************************************************************************
 * OTHER ACTIONS  INTERACTION
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

void AMIR_ArcadeController::SetSummonedMagicRef(AMIR_MagicBASE* const& MagicType)
{
	SummonedMagicRef = MagicType;
}

AMIR_MagicBASE* AMIR_ArcadeController::GetSummonedMagicRef() const
{
	return SummonedMagicRef;
}
