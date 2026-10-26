// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Input/MIR_ArcadeController.h"
#include "EnhancedInputComponent.h"
#include "Interfaces/IMIR_Interactable.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Components/MIR_ManaComponent.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Components/ArrowComponent.h"
#include "Magic/MIR_MagicBASE.h"
#include "Magic/Melee/MIR_MagicMelee.h"
#include "Magic/Ranged/MIR_MagicRanged.h"
#include "Magic/AOE/MIR_MagicAOE.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequenceBase.h"
#include "UObject/UnrealType.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"


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
		// Movement remains Triggered for continuous analog/WASD reading
		EnhancedInput->BindAction(Move, ETriggered, this, &AMIR_ArcadeController::MoveCharacter);

		// Attacks are bound to Started and Completed to track held state for hold-to-repeat
		EnhancedInput->BindAction(MainAttack, EStarted, this,
			&AMIR_ArcadeController::OnMainAttackStarted);
		EnhancedInput->BindAction(MainAttack, ECompleted, this,
			&AMIR_ArcadeController::OnMainAttackCompleted);

		EnhancedInput->BindAction(RangedCopy, EStarted, this,
			&AMIR_ArcadeController::OnRangedCopyStarted);
		EnhancedInput->BindAction(RangedCopy, ECompleted, this,
			&AMIR_ArcadeController::OnRangedCopyCompleted);

		EnhancedInput->BindAction(SubAttack, EStarted, this,
			&AMIR_ArcadeController::OnSubAttackStarted);
		EnhancedInput->BindAction(SubAttack, ECompleted, this,
			&AMIR_ArcadeController::OnSubAttackCompleted);

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
		World->GetTimerManager().ClearTimer(AttackSafetyUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(AttackStateResetTimerHandle);
		World->GetTimerManager().ClearTimer(MagicSpawnFallbackTimerHandle);
	}

	bIsMainAttackHeld = false;
	bIsRangedCopyHeld = false;
	bIsSubAttackHeld = false;

	Super::EndPlay(EndPlayReason);
}


/************************************************************************************
 * HELPER FUNCTIONS
 ***********************************************************************************/

AMIR_PlayerChar* AMIR_ArcadeController::GetPlayerCharacter() const
{
	if (AMIR_PlayerChar* PC = Cast<AMIR_PlayerChar>(GetCharacter()))
	{
		return PC;
	}
	if (AMIR_PlayerChar* PP = Cast<AMIR_PlayerChar>(GetPawn()))
	{
		return PP;
	}
	if (UWorld* World = GetWorld())
	{
		return Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}
	return nullptr;
}

/************************************************************************************
 * MOBILITY ACTIONS — GENERAL MOVEMENT
 ***********************************************************************************/

void AMIR_ArcadeController::MoveCharacter(const FInputActionValue& ActionValue)
{
	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter) { return; }

	UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement();
	const bool bIsAirborne = MoveComp && MoveComp->IsFalling();

	// Rooting only applies on the ground.
	// Jumps and airborne momentum are completely unaffected by attack animations.
	if (!bIsAirborne)
	{
		if (!bCanMove || CurrentAttackState != EAttackTypes::None)
		{
			return;
		}

		// If player is holding an attack button and the attack is ready to repeat,
		// keep Mira rooted between repeat swings.
		if ((bIsMainAttackHeld && CanPerformAttack(EMagicAttacks::MainAttack)) ||
			(bIsRangedCopyHeld && CanPerformAttack(EMagicAttacks::RangedCopy)) ||
			(bIsSubAttackHeld && CanPerformAttack(EMagicAttacks::SubAttack)))
		{
			return;
		}
	}

	FRotator ControllerRotation = PlayerCharacter->GetControlRotation();
	FVector ActionVector = ActionValue.Get<FVector>();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControllerRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControllerRotation);

	PlayerCharacter->AddMovementInput(ForwardVector, ActionVector.Y);
	PlayerCharacter->AddMovementInput(RightVector, ActionVector.X);
}

/************************************************************************************
 * MOBILITY ACTIONS — JUMPING
 ***********************************************************************************/

void AMIR_ArcadeController::StartPlayerJumping()
{
	if (!IsPlayerJumping() || !bCanMove || CurrentAttackState != EAttackTypes::None)
	{
		return;
	}

	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		PlayerCharacter->Jump();
		bCanAnimateJump = true;

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(JumpAnimationTimerHandle);
			World->GetTimerManager().SetTimer(JumpAnimationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				bCanAnimateJump = false;
			}), 0.5f, false);
		}
	}
}

void AMIR_ArcadeController::StopPlayerJumping()
{
	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		PlayerCharacter->StopJumping();
	}
}

bool AMIR_ArcadeController::IsPlayerJumping() const
{
	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		return PlayerCharacter->bHasSkyboundShoes;
	}
	return false;
}

/************************************************************************************
 * MOBILITY ACTIONS — DASHING
 ***********************************************************************************/

void AMIR_ArcadeController::StartDashing()
{
	if (!IsPlayerDashing() || !bCanMove || CurrentAttackState != EAttackTypes::None)
	{
		return;
	}

	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		if (UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement())
		{
			CharacterMovement->MaxWalkSpeed = 900.0f;
		}
	}
}

void AMIR_ArcadeController::StopDashing()
{
	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		if (UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement())
		{
			CharacterMovement->MaxWalkSpeed = 600.0f;
		}
	}
}

bool AMIR_ArcadeController::IsPlayerDashing() const
{
	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		return PlayerCharacter->bHasSpiritStole;
	}
	return false;
}

/************************************************************************************
 * COMBAT ACTIONS — ATTACK INPUTS & STATE
 ***********************************************************************************/

void AMIR_ArcadeController::OnMainAttackStarted()
{
	bIsMainAttackHeld = true;
	HandleMainAttackInput();
}

void AMIR_ArcadeController::OnMainAttackCompleted()
{
	bIsMainAttackHeld = false;
}

void AMIR_ArcadeController::OnRangedCopyStarted()
{
	bIsRangedCopyHeld = true;
	HandleRangedCopyInput();
}

void AMIR_ArcadeController::OnRangedCopyCompleted()
{
	bIsRangedCopyHeld = false;
}

void AMIR_ArcadeController::OnSubAttackStarted()
{
	bIsSubAttackHeld = true;
	HandleSubAttackInput();
}

void AMIR_ArcadeController::OnSubAttackCompleted()
{
	bIsSubAttackHeld = false;
}

void AMIR_ArcadeController::CheckAutoRepeatAttack()
{
	// Cannot repeat if already in an active attack animation
	if (CurrentAttackState != EAttackTypes::None)
	{
		return;
	}

	// Priority 1: Main Attack
	if (bIsMainAttackHeld && CanPerformAttack(EMagicAttacks::MainAttack))
	{
		HandleMainAttackInput();
	}
	// Priority 2: Ranged Copy
	else if (bIsRangedCopyHeld && CanPerformAttack(EMagicAttacks::RangedCopy))
	{
		HandleRangedCopyInput();
	}
	// Priority 3: Sub Attack
	else if (bIsSubAttackHeld && CanPerformAttack(EMagicAttacks::SubAttack))
	{
		HandleSubAttackInput();
	}
}

void AMIR_ArcadeController::HandleMainAttackInput()
{
	// Reject attack input if attacks cannot be performed or character cannot move
	if (!bCanMove || !CanPerformAttack(EMagicAttacks::MainAttack))
	{
		return;
	}

	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();

	// Root Mira on the ground during attack animations; jumps and mid-air momentum are not affected
	if (PlayerCharacter)
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			if (!MoveComp->IsFalling())
			{
				SetCanMove(false);
				MoveComp->StopMovementImmediately();
			}
		}
	}

	// Lock both main and sub attacks during the attack animation to prevent spell overlap
	bCanUseMainAttack = false;
	bCanUseSubAttack = false;
	if (UWorld* World = GetWorld())
	{
		LastMainAttackTime = World->GetTimeSeconds();
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(SubAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(AttackSafetyUnlockTimerHandle);
		// Safety unlock fallback in case animation or notify is interrupted
		World->GetTimerManager().SetTimer(AttackSafetyUnlockTimerHandle, this,
			&AMIR_ArcadeController::OnAttackAnimationFinished, 2.0f, false);
	}

	UseMainAttack();
}

void AMIR_ArcadeController::HandleRangedCopyInput()
{
	// Reject attack input if attacks cannot be performed or character cannot move
	if (!bCanMove || !CanPerformAttack(EMagicAttacks::RangedCopy))
	{
		return;
	}

	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();

	// Root Mira on the ground during attack animations; jumps and mid-air momentum are not affected
	if (PlayerCharacter)
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			if (!MoveComp->IsFalling())
			{
				SetCanMove(false);
				MoveComp->StopMovementImmediately();
			}
		}
	}

	// Lock both main and sub attacks during the attack animation to prevent spell overlap
	bCanUseMainAttack = false;
	bCanUseSubAttack = false;
	if (UWorld* World = GetWorld())
	{
		LastMainAttackTime = World->GetTimeSeconds();
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(SubAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(AttackSafetyUnlockTimerHandle);
		// Safety unlock fallback in case animation or notify is interrupted
		World->GetTimerManager().SetTimer(AttackSafetyUnlockTimerHandle, this,
			&AMIR_ArcadeController::OnAttackAnimationFinished, 2.0f, false);
	}

	UseMirrorStaff();
}

void AMIR_ArcadeController::HandleSubAttackInput()
{
	// Sub-Attack cannot interrupt an ongoing attack or move lock
	if (!bCanMove || !CanPerformAttack(EMagicAttacks::SubAttack))
	{
		return;
	}

	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();

	// Root Mira on the ground during attack animations; jumps and mid-air momentum are not affected
	if (PlayerCharacter)
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			if (!MoveComp->IsFalling())
			{
				SetCanMove(false);
				MoveComp->StopMovementImmediately();
			}
		}
	}
	
	bCanUseSubAttack = false;
	bCanUseMainAttack = false;

	if (UWorld* World = GetWorld())
	{
		LastSubAttackTime = World->GetTimeSeconds();
		World->GetTimerManager().ClearTimer(AttackSafetyUnlockTimerHandle);
		World->GetTimerManager().SetTimer(AttackSafetyUnlockTimerHandle, this,
			&AMIR_ArcadeController::OnAttackAnimationFinished, 2.0f, false);
	}

	UseSubAttack();
}


void AMIR_ArcadeController::UnlockSubAttack()
{
	AllowSubAttack();
}

void AMIR_ArcadeController::AllowSubAttack()
{
	bCanUseSubAttack = true;
	SubAttackUnlockTimerHandle.Invalidate();

	CheckAutoRepeatAttack();
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
	// Do not unlock attacks while an attack animation is currently running.
	// OnAttackAnimationFinished will unlock attacks when the animation actually completes.
	if (CurrentAttackState != EAttackTypes::None)
	{
		return;
	}

	bCanUseMainAttack = true;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(AttackSafetyUnlockTimerHandle);
	}

	CheckAutoRepeatAttack();
}

/************************************************************************************
 * COMBAT ACTIONS — NATIVE ATTACK IMPLEMENTATIONS
 ***********************************************************************************/

void AMIR_ArcadeController::UseMainAttack_Implementation()
{
	if (!CanCastMagic(EMagicAttacks::MainAttack))
	{
		return;
	}

	CurrentMagicSlot = EMagicAttacks::MainAttack;
	TSubclassOf<AMIR_MagicBASE> MagicClass = GetAppropriateMagic(EMagicAttacks::MainAttack);
	if (!MagicClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMIR_ArcadeController: UseMainAttack found no MagicClass."));
		CurrentAttackState = EAttackTypes::None;
		UpdateAnimInstanceAttackType(EAttackTypes::None);
		AllowMainAttack();
		return;
	}

	CurrentAttackState = GetAttackTypeForMagic(MagicClass);
	UpdateAnimInstanceAttackType(CurrentAttackState);
	bHasSpawnedMagicThisAttack = false;

	// Start animation reset timer to prevent freezing at end of animation
	StartAttackAnimationTimer(CurrentAttackState);

	// Spawns precisely at the animation notify frame, with fallback timer
	StartSpawnFallbackTimer();
}

void AMIR_ArcadeController::UseMirrorStaff_Implementation()
{
	if (!CanCastMagic(EMagicAttacks::RangedCopy))
	{
		return;
	}

	CurrentMagicSlot = EMagicAttacks::RangedCopy;
	CurrentAttackState = EAttackTypes::Ranged;
	UpdateAnimInstanceAttackType(EAttackTypes::Ranged);
	bHasSpawnedMagicThisAttack = false;

	// Start animation reset timer so character transitions back to Idle/Run
	StartAttackAnimationTimer(EAttackTypes::Ranged);

	// Magic spawns at the notify frame, or via fallback timer if notify is unwired.
	StartSpawnFallbackTimer();
}

void AMIR_ArcadeController::UseSubAttack_Implementation()
{
	if (!CanCastMagic(EMagicAttacks::SubAttack))
	{
		return;
	}

	CurrentMagicSlot = EMagicAttacks::SubAttack;
	CurrentAttackState = EAttackTypes::Melee;
	UpdateAnimInstanceAttackType(EAttackTypes::Melee);
	bHasSpawnedMagicThisAttack = false;

	// Start animation reset timer for sub-attack (dagger)
	StartAttackAnimationTimer(EAttackTypes::Melee, true);

	// Spawns precisely at the animation notify frame, with fallback timer
	StartSpawnFallbackTimer();
}



void AMIR_ArcadeController::ResetCopiedMagic()
{
	if (AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter())
	{
		// The Magic Wand does not copy or discard spells; only Mirror Staff does
		if (!PlayerCharacter->bHasMirrorStaff)
		{
			return;
		}

		if (PlayerCharacter->GetStoredMagic() != nullptr)
		{
			PlayerCharacter->SetStoredMagic(nullptr);
			if (PlayerCharacter->ElementComponent)
			{
				PlayerCharacter->ElementComponent->SetElementDetails(EElements::None);
			}
			PlayerCharacter->CopyElement();
		}
	}
}

void AMIR_ArcadeController::DiscardCopiedMagic_Implementation()
{
	if (CurrentAttackState != EAttackTypes::None)
	{
		return;
	}

	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter || !PlayerCharacter->bHasMirrorStaff)
	{
		return; // Cannot discard if player does not have the Mirror Staff
	}

	ResetCopiedMagic();
	CurrentMagicSlot = EMagicAttacks::None;
	CurrentAttackState = EAttackTypes::None;
	UpdateAnimInstanceAttackType(EAttackTypes::None);
}

/************************************************************************************
 * COMBAT ACTIONS — MAGIC RESOLUTION & SPAWNING
 ***********************************************************************************/

TSubclassOf<AMIR_MagicBASE> AMIR_ArcadeController::GetAppropriateMagic(EMagicAttacks AttackSlot) const
{
	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return nullptr;
	}

	switch (AttackSlot)
	{
	case EMagicAttacks::SubAttack:
		return PlayerCharacter->GetDaggerRef();

	case EMagicAttacks::RangedCopy:
		return PlayerCharacter->GetRangedCopyRef();

	case EMagicAttacks::MainAttack:
	{
		TSubclassOf<AMIR_MagicBASE> Stored = PlayerCharacter->GetStoredMagic();
		if (Stored)
		{
			return Stored;
		}
		// If player does not have the Mirror Staff, they cannot use MeleeCopy!
		if (!PlayerCharacter->bHasMirrorStaff)
		{
			return nullptr;
		}
		return PlayerCharacter->GetMeleeCopyRef();
	}

	default:
		return nullptr;
	}
}

EAttackTypes AMIR_ArcadeController::GetAttackTypeForMagic(TSubclassOf<AMIR_MagicBASE> MagicClass) const
{
	if (!MagicClass)
	{
		return EAttackTypes::None;
	}

	if (MagicClass->IsChildOf(AMIR_MagicMelee::StaticClass()))
	{
		return EAttackTypes::Melee;
	}
	if (MagicClass->IsChildOf(AMIR_MagicRanged::StaticClass()))
	{
		return EAttackTypes::Ranged;
	}
	if (MagicClass->IsChildOf(AMIR_MagicAOE::StaticClass()))
	{
		return EAttackTypes::AoE;
	}

	if (AMIR_MagicBASE* CDO = MagicClass->GetDefaultObject<AMIR_MagicBASE>())
	{
		if (CDO->AttackTypeComponent)
		{
			return CDO->AttackTypeComponent->GetAttackTypeName();
		}
	}

	return EAttackTypes::None;
}

int32 AMIR_ArcadeController::GetManaCostForMagic(TSubclassOf<AMIR_MagicBASE> MagicClass) const
{
	if (!MagicClass)
	{
		return 0;
	}

	if (AMIR_MagicBASE* CDO = MagicClass->GetDefaultObject<AMIR_MagicBASE>())
	{
		if (CDO->AttackTypeComponent)
		{
			// Copy actions (e.g. MeleeCopy or RangedCopy) have property Copy and cost 0
			if (CDO->AttackTypeComponent->GetAttackTypeProperty() == EAttackProperties::Copy)
			{
				return 0;
			}

			int32 Cost = CDO->AttackTypeComponent->GetManaConsumption();
			if (Cost > 0)
			{
				return Cost;
			}
		}
	}

	// Class-based fallback if component wasn't initialized or returned default
	if (MagicClass->IsChildOf(AMIR_MagicAOE::StaticClass()))
	{
		return 3;
	}
	if (MagicClass->IsChildOf(AMIR_MagicMelee::StaticClass()))
	{
		return 2;
	}
	if (MagicClass->IsChildOf(AMIR_MagicRanged::StaticClass()))
	{
		return 1;
	}

	return 0;
}

bool AMIR_ArcadeController::CanCastMagic(EMagicAttacks AttackSlot) const
{
	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return false;
	}

	if (AttackSlot == EMagicAttacks::RangedCopy && !PlayerCharacter->bHasMirrorStaff)
	{
		return false;
	}

	TSubclassOf<AMIR_MagicBASE> MagicClass = GetAppropriateMagic(AttackSlot);
	if (!MagicClass)
	{
		return false;
	}

	// Sub-Attack (dagger) consumes no mana
	if (AttackSlot == EMagicAttacks::SubAttack)
	{
		return true;
	}

	if (PlayerCharacter->ManaComponent)
	{
		// During mana cooldown, no magic can be cast
		if (PlayerCharacter->ManaComponent->IsCooldownActive())
		{
			return false;
		}

		// If mana is completely depleted, no magic can be cast
		if (PlayerCharacter->ManaComponent->ManaPower <= 0)
		{
			return false;
		}

		// As long as ManaPower > 0, the attack is allowed; low mana will clamp down to 0
	}

	return true;
}

bool AMIR_ArcadeController::CanPerformAttack(EMagicAttacks AttackSlot) const
{
	if (CurrentAttackState != EAttackTypes::None)
	{
		return false;
	}

	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return false;
	}

	switch (AttackSlot)
	{
	case EMagicAttacks::MainAttack:
	{
		if (!bCanUseMainAttack)
		{
			return false;
		}
		return CanCastMagic(EMagicAttacks::MainAttack);
	}

	case EMagicAttacks::RangedCopy:
	{
		if (!bCanUseMainAttack || !PlayerCharacter->bHasMirrorStaff)
		{
			return false;
		}
		return CanCastMagic(EMagicAttacks::RangedCopy);
	}

	case EMagicAttacks::SubAttack:
	{
		if (!bCanUseSubAttack)
		{
			return false;
		}
		return CanCastMagic(EMagicAttacks::SubAttack);
	}

	default:
		return false;
	}
}

EAttackTypes AMIR_ArcadeController::GetAttackTypeName() const
{
	if (CurrentAttackState != EAttackTypes::None)
	{
		return CurrentAttackState;
	}

	// Dynamic fallback if CurrentAttackState wasn't explicitly assigned
	if (CurrentMagicSlot != EMagicAttacks::None)
	{
		TSubclassOf<AMIR_MagicBASE> MagicClass = GetAppropriateMagic(CurrentMagicSlot);
		return GetAttackTypeForMagic(MagicClass);
	}

	return EAttackTypes::None;
}

UArrowComponent* AMIR_ArcadeController::GetCorrectSpawnArrow(EAttackTypes AttackType) const
{
	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return nullptr;
	}

	switch (AttackType)
	{
	case EAttackTypes::Melee:
		return PlayerCharacter->MeleeArrow;
	case EAttackTypes::Ranged:
		return PlayerCharacter->RangedArrow;
	case EAttackTypes::AoE:
		return PlayerCharacter->AOEArrow;
	default:
		return PlayerCharacter->MeleeArrow;
	}
}

AMIR_MagicBASE* AMIR_ArcadeController::SpawnMagic_Implementation(EMagicAttacks AttackSlot)
{
	if (bHasSpawnedMagicThisAttack)
	{
		return nullptr;
	}

	if (!CanCastMagic(AttackSlot))
	{
		return nullptr;
	}

	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	TSubclassOf<AMIR_MagicBASE> MagicClass = GetAppropriateMagic(AttackSlot);
	if (!MagicClass)
	{
		return nullptr;
	}

	// Deduct mana (SubAttack dagger does not consume mana)
	if (AttackSlot != EMagicAttacks::SubAttack)
	{
		const int32 ManaCost = GetManaCostForMagic(MagicClass);

		if (PlayerCharacter->ManaComponent)
		{
			if (PlayerCharacter->ManaComponent->IsCooldownActive())
			{
				return nullptr;
			}

			if (PlayerCharacter->ManaComponent->ManaPower <= 0)
			{
				return nullptr;
			}

			if (ManaCost > 0)
			{
				// Clamp deduction: consume remaining mana, clamping to 0 if ManaPower <= ManaCost
				if (PlayerCharacter->ManaComponent->ManaPower <= ManaCost)
				{
					PlayerCharacter->ManaComponent->SetManaPower(0);
				}
				else
				{
					PlayerCharacter->ManaComponent->ManaReduction(ManaCost);
				}

				// If mana reaches 0, initiate cooldown and lose copied magic
				if (PlayerCharacter->ManaComponent->ManaPower <= 0)
				{
					PlayerCharacter->ManaComponent->ManaCooldown();

					// Lose copied spell on mana depletion to encourage copying more spells
					ResetCopiedMagic();
					bIsMainAttackHeld = false;
					bIsRangedCopyHeld = false;
				}
			}
		}
	}

	EAttackTypes AttackType = GetAttackTypeForMagic(MagicClass);
	UArrowComponent* SpawnArrow = GetCorrectSpawnArrow(AttackType);
	FTransform SpawnTransform = SpawnArrow ? SpawnArrow->GetComponentTransform() : PlayerCharacter->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.Instigator = PlayerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMIR_MagicBASE* SpawnedMagic = World->SpawnActor<AMIR_MagicBASE>(MagicClass, SpawnTransform, SpawnParams);
	if (!SpawnedMagic)
	{
		return nullptr;
	}

	bHasSpawnedMagicThisAttack = true;

	// Attachment: ONLY Melee magic is attached to the player's MeleeArrow!

	// Ranged and AoE remain in world space so they are never glued to the player.
	if (AttackType == EAttackTypes::Melee && SpawnArrow)
	{
		SpawnedMagic->AttachToComponent(SpawnArrow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	// Optional sound effects
	USoundBase* SoundToPlay = nullptr;
	if (AttackSlot == EMagicAttacks::SubAttack)
	{
		SoundToPlay = DaggerAttackSound;
	}
	else if (AttackSlot == EMagicAttacks::RangedCopy)
	{
		SoundToPlay = MirrorStaffAttackSound;
	}
	else
	{
		switch (AttackType)
		{
		case EAttackTypes::Melee:
			SoundToPlay = MeleeAttackSound;
			break;
		case EAttackTypes::Ranged:
			SoundToPlay = RangedAttackSound;
			break;
		case EAttackTypes::AoE:
			SoundToPlay = AOEAttackSound;
			break;
		default:
			break;
		}
	}

	if (SoundToPlay)
	{
		UGameplayStatics::PlaySound2D(this, SoundToPlay);
	}

	SetSummonedMagicRef(SpawnedMagic);
	return SpawnedMagic;
}

void AMIR_ArcadeController::SpawnRangedProjectileFromNotify()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MagicSpawnFallbackTimerHandle);
	}

	if (CurrentMagicSlot != EMagicAttacks::None)
	{
		SpawnMagic(CurrentMagicSlot);
	}
}

void AMIR_ArcadeController::SpawnAOEFromNotify()
{
	SpawnRangedProjectileFromNotify();
}

void AMIR_ArcadeController::TriggerSpawnFromNotify()
{
	SpawnRangedProjectileFromNotify();
}

void AMIR_ArcadeController::OnAttackLeftFromNotify()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AttackStateResetTimerHandle);
	}
	OnAttackAnimationFinished();
}

void AMIR_ArcadeController::AttackLeftFromNotify()
{
	OnAttackLeftFromNotify();
}

void AMIR_ArcadeController::StartAttackAnimationTimer(EAttackTypes AttackType, bool bIsSubAttack)

{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	float AnimDuration = 0.6f;
	if (AMIR_PlayerChar* PlayerChar = GetPlayerCharacter())
	{
		UAnimSequenceBase* Anim = bIsSubAttack ? PlayerChar->GetSubAttackAnimation() : PlayerChar->GetAttackAnimation(AttackType);
		if (Anim)
		{
			// Check if the animation has an AttackLeft notify to exit at the exact notify timestamp
			bool bFoundNotify = false;
			for (const FAnimNotifyEvent& NotifyEvent : Anim->Notifies)
			{
				if (NotifyEvent.NotifyName == FName("AttackLeft"))
				{
					AnimDuration = NotifyEvent.GetTime();
					bFoundNotify = true;
					break;
				}
			}

			if (!bFoundNotify)
			{
				AnimDuration = Anim->GetPlayLength();
			}
		}
	}

	World->GetTimerManager().ClearTimer(AttackStateResetTimerHandle);
	World->GetTimerManager().SetTimer(AttackStateResetTimerHandle, this,
		&AMIR_ArcadeController::OnAttackAnimationFinished, AnimDuration, false);
}

void AMIR_ArcadeController::OnAttackAnimationFinished()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AttackSafetyUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(AttackStateResetTimerHandle);
		World->GetTimerManager().ClearTimer(MagicSpawnFallbackTimerHandle);
		World->GetTimerManager().ClearTimer(SubAttackUnlockTimerHandle);
		World->GetTimerManager().ClearTimer(MainAttackUnlockTimerHandle);
	}

	ResetAttackState();

	// Immediately re-enable both attack slots
	bCanUseMainAttack = true;
	bCanUseSubAttack = true;
	SetCanMove(true);

	// If any attack button is held and can actually be performed, chain into the next attack on the next tick.
	// We wait 1 tick so the AnimBP state machine can process the transition out of
	// the current attack state before re-entering, which avoids the T-pose pop.
	// During this 1-tick gap, MoveCharacter's held-input guard keeps Mira rooted.
	if ((bIsMainAttackHeld && CanPerformAttack(EMagicAttacks::MainAttack)) ||
		(bIsRangedCopyHeld && CanPerformAttack(EMagicAttacks::RangedCopy)) ||
		(bIsSubAttackHeld && CanPerformAttack(EMagicAttacks::SubAttack)))
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(this, &AMIR_ArcadeController::CheckAutoRepeatAttack);
		}
		else
		{
			CheckAutoRepeatAttack();
		}
	}
}

void AMIR_ArcadeController::StartSpawnFallbackTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(MagicSpawnFallbackTimerHandle);

	float SpawnDelay = -1.0f;
	if (AMIR_PlayerChar* PlayerChar = GetPlayerCharacter())
	{
		bool bIsSubAttack = (CurrentMagicSlot == EMagicAttacks::SubAttack);
		UAnimSequenceBase* Anim = bIsSubAttack ? PlayerChar->GetSubAttackAnimation() : PlayerChar->GetAttackAnimation(CurrentAttackState);
		if (Anim)
		{
			for (const FAnimNotifyEvent& NotifyEvent : Anim->Notifies)
			{
				if (NotifyEvent.NotifyName == FName("SpawnRangedMagic") ||
					NotifyEvent.NotifyName == FName("SpawnMagic") ||
					NotifyEvent.NotifyName == FName("SpawnRangedProjectile"))
				{
					SpawnDelay = NotifyEvent.GetTime();
					break;
				}
			}

			if (SpawnDelay < 0.0f)
			{
				float PlayLength = Anim->GetPlayLength();
				if (CurrentAttackState == EAttackTypes::AoE)
				{
					SpawnDelay = PlayLength * 0.55f;
				}
				else if (CurrentAttackState == EAttackTypes::Ranged)
				{
					SpawnDelay = PlayLength * 0.4f;
				}
				else
				{
					SpawnDelay = PlayLength * 0.35f;
				}
			}
		}
	}

	if (SpawnDelay <= 0.0f)
	{
		if (CurrentAttackState == EAttackTypes::AoE)
		{
			SpawnDelay = 0.7f;
		}
		else if (CurrentAttackState == EAttackTypes::Ranged)
		{
			SpawnDelay = 0.35f;
		}
		else
		{
			SpawnDelay = 0.2f;
		}
	}

	World->GetTimerManager().SetTimer(MagicSpawnFallbackTimerHandle, this,
		&AMIR_ArcadeController::TriggerSpawnFallback, SpawnDelay, false);
}

void AMIR_ArcadeController::TriggerSpawnFallback()
{
	if (CurrentMagicSlot != EMagicAttacks::None)
	{
		SpawnMagic(CurrentMagicSlot);
	}
}

void AMIR_ArcadeController::UpdateAnimInstanceAttackType(EAttackTypes NewAttackType)
{
	AMIR_PlayerChar* PlayerChar = GetPlayerCharacter();
	if (!PlayerChar)
	{
		return;
	}

	USkeletalMeshComponent* Mesh = PlayerChar->GetMesh();
	if (!Mesh)
	{
		return;
	}

	UAnimInstance* AnimInst = Mesh->GetAnimInstance();
	if (!AnimInst)
	{
		return;
	}

	const FName PropNames[] = {
		FName("CurrentAttackType"),
		FName("CurrentAttackState"),
		FName("Current Attack Type"),
		FName("Current Attack State"),
		FName("AttackType"),
		FName("AttackState")
	};

	for (const FName& PropName : PropNames)
	{
		if (FProperty* Prop = AnimInst->GetClass()->FindPropertyByName(PropName))
		{
			if (FByteProperty* ByteProp = CastField<FByteProperty>(Prop))
			{
				ByteProp->SetPropertyValue_InContainer(AnimInst, static_cast<uint8>(NewAttackType));
			}
			else if (FEnumProperty* EnumProp = CastField<FEnumProperty>(Prop))
			{
				if (FNumericProperty* UnderlyingProp = EnumProp->GetUnderlyingProperty())
				{
					void* PropAddr = EnumProp->ContainerPtrToValuePtr<void>(AnimInst);
					UnderlyingProp->SetIntPropertyValue(PropAddr, static_cast<int64>(NewAttackType));
				}
			}
		}
	}
}


void AMIR_ArcadeController::ResetAttackState()
{
	CurrentAttackState = EAttackTypes::None;
	CurrentMagicSlot = EMagicAttacks::None;
	bHasSpawnedMagicThisAttack = false;
	UpdateAnimInstanceAttackType(EAttackTypes::None);
}


void AMIR_ArcadeController::SetCanMove(bool bInCanMove)
{
	bCanMove = bInCanMove;
}


/************************************************************************************
 * OTHER ACTIONS — INTERACTION & REFERENCES
 ***********************************************************************************/

void AMIR_ArcadeController::UseInteract()
{
	AMIR_PlayerChar* PlayerCharacter = GetPlayerCharacter();
	if (!PlayerCharacter)
	{
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(PlayerCharacter);

	TArray<AActor*> NearbyActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerCharacter->GetActorLocation(),
		InteractRange, ObjectTypes, AActor::StaticClass(), IgnoredActors, NearbyActors);

	AActor* ClosestInteractable = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (AActor* NearbyActor : NearbyActors)
	{
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
	return IsValid(SummonedMagicRef) ? SummonedMagicRef : nullptr;
}
