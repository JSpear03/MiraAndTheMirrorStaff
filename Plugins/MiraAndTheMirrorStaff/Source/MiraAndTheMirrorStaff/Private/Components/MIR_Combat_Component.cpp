// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Components/MIR_Combat_Component.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMIR_Combat_Component::UMIR_Combat_Component()
{
	PrimaryComponentTick.bCanEverTick = false; // No tick loop needed! Clean performance.
}

void UMIR_Combat_Component::ExecuteMeleeAttack()
{
	bAttackFinished = false; // Lock the task loop.
	bHasSpawnedMagicThisAttack = false;

	if (bUseFallbackSpawnTimer)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				FallbackSpawnTimerHandle,
				this,
				&UMIR_Combat_Component::TriggerMagicSpawnFromNotify,
				FallbackMagicSpawnDelay,
				false
			);
		}
	}

	OnMeleeAttackTriggered.Broadcast();
}

void UMIR_Combat_Component::ExecuteMagicAttack()
{
	bAttackFinished = false; // Lock the task loop.
	bHasSpawnedMagicThisAttack = false;

	if (bUseFallbackSpawnTimer)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				FallbackSpawnTimerHandle,
				this,
				&UMIR_Combat_Component::TriggerMagicSpawnFromNotify,
				FallbackMagicSpawnDelay,
				false
			);
		}
	}

	OnMagicAttackTriggered.Broadcast();
}

void UMIR_Combat_Component::TriggerMagicSpawnFromNotify()
{
	if (bIsDeactivated) { return; }

	// Clear safety-fallback timer since the anim notify has fired.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FallbackSpawnTimerHandle);
	}

	bHasSpawnedMagicThisAttack = true;
	OnSpawnMagicFromNotify.Broadcast();
}

void UMIR_Combat_Component::FinishCurrentAttack()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FallbackSpawnTimerHandle);
	}

	bAttackFinished = true; // Unlock the task loop.
	OnAttackFinished.Broadcast();
}

void UMIR_Combat_Component::SetDeactivated(bool bNewDeactivated)
{
	bIsDeactivated = bNewDeactivated;
	if (bIsDeactivated)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(FallbackSpawnTimerHandle);
		}
	}
}

void UMIR_Combat_Component::ReactivateCombatState()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FallbackSpawnTimerHandle);
	}

	bIsDeactivated = false; // Enemy is back to life.
	bAttackFinished = true; // Clean, default slate for attacks.
	bHasSpawnedMagicThisAttack = false;
}
