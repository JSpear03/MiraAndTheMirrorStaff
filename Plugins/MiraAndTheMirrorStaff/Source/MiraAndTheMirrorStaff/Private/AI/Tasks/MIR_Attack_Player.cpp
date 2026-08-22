// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MIR_Attack_Player.h"

UMIR_Attack_Player::UMIR_Attack_Player() {
	NodeName = "Attack Player";
	SelectedAttackMode = ECombatAttackMode::Melee;
	AttackTimeout = 2.0f;
	bNotifyTick = true;		// Needed to use TickTask
}


FString UMIR_Attack_Player::GetStaticDescription() const
{
	FString ModeText = "Choose a 'Selected Attack Mode'";

	// Convert the enum state into a readable word
	switch (SelectedAttackMode)
	{
	case ECombatAttackMode::Melee:
		ModeText = "Melee";
		break;
	case ECombatAttackMode::Magic:
		ModeText = "Magic";
		break;
	}

	// Format the display text (e.g., "Attack Player: [Melee]")
	// Using Super::GetStaticDescription() keeps the baseline Behavior Tree label 
	// and appends our custom variable setting below it cleanly.
	return FString::Printf(TEXT("%s\nMode: %s"), *Super::GetStaticDescription(), *ModeText);
}

/*          DEVELOPER & DESIGNER DOCUMENTATION NOTES            *
*
*   =================================================================
*   PART 1: BASE CLASS EDITS (For the Gameplay Engineer)
*   =================================================================
*   When implementing the death/pooling function inside 'MIR_EnemyBASE.cpp',
*   we must explicitly update our custom combat component so active
*   Behavior Tree tasks break immediately instead of freezing in memory.
*
*   Add this logic to the bottom of AMIR_EnemyBASE::EnemyDied():
*
*   if (UMIR_Combat_Component* CombatComp = FindComponentByClass<UMIR_Combat_Component>())
*   {
*       CombatComp->SetDeactivated(true);
*   }
*
*   =================================================================
*   PART 2: BLUEPRINT WIREUP GUIDE (For the Technical Designer)
*   =================================================================
*   To enable combat functionality on any Enemy Blueprint:
*
*   1. Click 'Add Component' and attach the 'MIR_Combat_Component' 
*	(if not already).
*   2. In your Event Graph, right-click or use the details panel to add:
*      - 'Event On Melee Attack Triggered'
*      - 'Event On Magic Attack Triggered'
*   3. Connect those events to your custom attack logic (Anim Montages,
*      spawning projectiles, or routing to your Magic Component).
*   4. CRITICAL CLEANUP STEP: At the absolute end of your attack sequence
*      (or from a finishing Anim Notify), drag out a reference to the
*      'MIR_Combat_Component' and call the function node:
*      'Finish Current Attack'.
*
*   =================================================================
*   PART 3: RESPONDING / POOL RESET (For Spawner Integration)
*   =================================================================
*   Whenever a pooling manager pulls a hidden enemy out of memory and
*   wakes it up, reset the combat states by calling this node on the
*   component right before restarting the AI logic:
*
*   'Reactivate Combat State'
*
*/


EBTNodeResult::Type UMIR_Attack_Player::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Reset memory timer
	FBTAttackPlayerMemory* MyMemory = CastInstanceNodeMemory<FBTAttackPlayerMemory>(NodeMemory);
	if (MyMemory)
	{
		MyMemory->ElapsedTime = 0.0f;
	}

	// Get a generic pawn actor
	APawn* GenericPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	AMIR_EnemyBASE* ControlledPawn = Cast<AMIR_EnemyBASE>(GenericPawn);

	if (!ControlledPawn) {
		return EBTNodeResult::Failed;
	}

	// Grab combat component from current pawn
	UMIR_Combat_Component* CombatComp = GenericPawn->FindComponentByClass<UMIR_Combat_Component>();
	if (!CombatComp) {
		return EBTNodeResult::Failed;
	}

	// Read parameter set in editor node layout
	switch (SelectedAttackMode)
	{
		case ECombatAttackMode::Melee:
			CombatComp->ExecuteMeleeAttack();
			break;

		case ECombatAttackMode::Magic:
			CombatComp->ExecuteMagicAttack();
			break;
	}

	// Return in Progress to let animation sequence play
	return EBTNodeResult::InProgress;
}

void UMIR_Attack_Player::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* GenericPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	AMIR_EnemyBASE* ControlledPawn = Cast<AMIR_EnemyBASE>(GenericPawn);

	if (!ControlledPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Grab combat component from current pawn
	UMIR_Combat_Component* CombatComp = GenericPawn->FindComponentByClass<UMIR_Combat_Component>();

	if (CombatComp)
	{
		// 1. If the character has been deactivated/pooled, abort out cleanly right away
		if (CombatComp->IsDeactivated())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// 2. Otherwise, check if the normal attack sequence has finished playing
		if (CombatComp->IsAttackAnimationFinished())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// 3. Fallback Safety Timeout: Prevent AI from freezing permanently if animation/notify is missing
		FBTAttackPlayerMemory* MyMemory = CastInstanceNodeMemory<FBTAttackPlayerMemory>(NodeMemory);
		if (MyMemory)
		{
			MyMemory->ElapsedTime += DeltaSeconds;
			if (AttackTimeout > 0.0f && MyMemory->ElapsedTime >= AttackTimeout)
			{
				CombatComp->FinishCurrentAttack();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
	}
	else
	{
		// Failsafe if the component is missing entirely
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}