// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/MIR_AlignWithPlayer.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


UMIR_AlignWithPlayer::UMIR_AlignWithPlayer()
{
	NodeName = "Align With Player";
	AlignmentTolerance = 50.0f;
	AcceptableRadius = 30.0f;
	AlignmentTimeout = 4.0f;
	bNotifyTick = true;		// Needed to poll MoveToLocation progress

	// Filter key selector to only accept Actors/Objects (the player character)
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMIR_AlignWithPlayer, BlackboardKey), AActor::StaticClass());
}


FString UMIR_AlignWithPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s\nTolerance: %.1f\nRadius: %.1f\nTimeout: %.1fs"),
		*Super::GetStaticDescription(),
		AlignmentTolerance,
		AcceptableRadius,
		AlignmentTimeout);
}


/**
 * Calculates the alignment destination and initiates movement toward it.
 *
 * Per the GDD, the Mini-Dragon should "try to align itself with the player
 * on either the x- or y-axis (whichever is more distant from the player)."
 *
 * The algorithm:
 *   1. Compute the absolute delta on X and Y between the enemy and the player.
 *   2. Pick the axis with the LARGER delta (the one where the enemy is more distant).
 *   3. Build a target location that keeps the enemy's current position on the
 *      axis it is already close on, and matches the player's coordinate on the
 *      axis where it is far away.
 *   4. Issue MoveToLocation toward that target.
 *
 * Result: the enemy slides to align on the axis where the gap is greatest,
 * giving it a clear line of sight for its ranged projectile.
 */
EBTNodeResult::Type UMIR_AlignWithPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTAlignWithPlayerMemory* MyMemory = CastInstanceNodeMemory<FBTAlignWithPlayerMemory>(NodeMemory);
	if (MyMemory)
	{
		MyMemory->ElapsedTime = 0.0f;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 1. Try to get target player from the Blackboard key
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AMIR_PlayerChar* PlayerActor = nullptr;

	if (BlackboardComp)
	{
		PlayerActor = Cast<AMIR_PlayerChar>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));
	}

	// 2. Fallback to Player 0 if Blackboard key is not set
	if (!PlayerActor)
	{
		PlayerActor = Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(ControlledPawn->GetWorld(), 0));
	}

	if (!PlayerActor)
	{
		return EBTNodeResult::Failed;
	}

	FVector EnemyLocation = ControlledPawn->GetActorLocation();
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	float DeltaX = FMath::Abs(EnemyLocation.X - PlayerLocation.X);
	float DeltaY = FMath::Abs(EnemyLocation.Y - PlayerLocation.Y);

	FVector AlignmentTarget;
	float OffAxisDistance = 0.0f;
	float CardinalYaw = 0.0f;

	if (DeltaX >= DeltaY)
	{
		// Firing lane is along the X-axis (more distant on X)
		// Enemy must slide along Y to match Player.Y
		AlignmentTarget = FVector(EnemyLocation.X, PlayerLocation.Y, EnemyLocation.Z);
		OffAxisDistance = DeltaY;
		CardinalYaw = (PlayerLocation.X >= EnemyLocation.X) ? 0.0f : 180.0f;
	}
	else
	{
		// Firing lane is along the Y-axis (more distant on Y)
		// Enemy must slide along X to match Player.X
		AlignmentTarget = FVector(PlayerLocation.X, EnemyLocation.Y, EnemyLocation.Z);
		OffAxisDistance = DeltaX;
		CardinalYaw = (PlayerLocation.Y >= EnemyLocation.Y) ? 90.0f : -90.0f;
	}

	// 3. Check if already precisely aligned within tolerance
	if (OffAxisDistance <= AlignmentTolerance)
	{
		// Already aligned — snap to pure cardinal axis facing the player
		ControlledPawn->SetActorRotation(FRotator(0.0f, CardinalYaw, 0.0f));
		return EBTNodeResult::Succeeded;
	}

	// 4. Move to the alignment destination
	AIController->MoveToLocation(AlignmentTarget, 5.0f, true, true, false, false);

	return EBTNodeResult::InProgress;
}


void UMIR_AlignWithPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 1. Get current player position (dynamically tracks real-time movement)
	AActor* PlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent() ? 
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()) : nullptr);

	if (!PlayerActor)
	{
		PlayerActor = UGameplayStatics::GetPlayerCharacter(ControlledPawn->GetWorld(), 0);
	}

	if (!PlayerActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector EnemyLocation = ControlledPawn->GetActorLocation();
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	float DeltaX = FMath::Abs(EnemyLocation.X - PlayerLocation.X);
	float DeltaY = FMath::Abs(EnemyLocation.Y - PlayerLocation.Y);

	FVector AlignmentTarget;
	float OffAxisDistance = 0.0f;
	float CardinalYaw = 0.0f;

	if (DeltaX >= DeltaY)
	{
		// Firing lane along X -> align on Y
		AlignmentTarget = FVector(EnemyLocation.X, PlayerLocation.Y, EnemyLocation.Z);
		OffAxisDistance = DeltaY;
		CardinalYaw = (PlayerLocation.X >= EnemyLocation.X) ? 0.0f : 180.0f;
	}
	else
	{
		// Firing lane along Y -> align on X
		AlignmentTarget = FVector(PlayerLocation.X, EnemyLocation.Y, EnemyLocation.Z);
		OffAxisDistance = DeltaX;
		CardinalYaw = (PlayerLocation.Y >= EnemyLocation.Y) ? 90.0f : -90.0f;
	}

	// 2. Check if we have achieved precise alignment
	if (OffAxisDistance <= AlignmentTolerance)
	{
		AIController->StopMovement();
		ControlledPawn->SetActorRotation(FRotator(0.0f, CardinalYaw, 0.0f));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 3. Update destination dynamically to track player movement
	AIController->MoveToLocation(AlignmentTarget, 5.0f, true, true, false, false);

	// 4. Timeout / stuck failsafe
	FBTAlignWithPlayerMemory* MyMemory = CastInstanceNodeMemory<FBTAlignWithPlayerMemory>(NodeMemory);
	if (MyMemory)
	{
		MyMemory->ElapsedTime += DeltaSeconds;
		if (AlignmentTimeout > 0.0f && MyMemory->ElapsedTime >= AlignmentTimeout)
		{
			AIController->StopMovement();
			ControlledPawn->SetActorRotation(FRotator(0.0f, CardinalYaw, 0.0f));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}


EBTNodeResult::Type UMIR_AlignWithPlayer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		// Stop any active pathfinding when the task is externally aborted
		AIController->StopMovement();
	}

	return EBTNodeResult::Aborted;
}
