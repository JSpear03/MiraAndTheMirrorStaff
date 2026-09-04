// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MIR_Combat_Component.h"

UMIR_Combat_Component::UMIR_Combat_Component()
{
	PrimaryComponentTick.bCanEverTick = false; // No tick loop needed! Clean performance.
}

void UMIR_Combat_Component::ExecuteMeleeAttack()
{
	bAttackFinished = false; // Lock the task loop
	OnMeleeAttackTriggered.Broadcast();
}

void UMIR_Combat_Component::ExecuteMagicAttack()
{
	bAttackFinished = false; // Lock the task loop
	OnMagicAttackTriggered.Broadcast();
}


void UMIR_Combat_Component::FinishCurrentAttack()
{
	bAttackFinished = true; // Unlock the task loop
}

void UMIR_Combat_Component::ReactivateCombatState()
{
	bIsDeactivated = false; // Enemy is back to life
	bAttackFinished = true; // Clean default slate for attacks
}
