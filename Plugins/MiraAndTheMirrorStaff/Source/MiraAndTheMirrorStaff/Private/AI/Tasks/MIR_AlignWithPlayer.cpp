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
bool UMIR_AlignWithPlayer::HasLineOfSight(UWorld* World, const FVector& FromLocation, const AActor* TargetActor, const AActor* IgnoredPawn) const
{
	if (!World || !TargetActor)
	{
		return false;
	}

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AlignLineOfSight), true);
	TraceParams.AddIgnoredActor(IgnoredPawn);
	TraceParams.AddIgnoredActor(TargetActor);

	FVector StartPos = FromLocation;
	FVector EndPos = TargetActor->GetActorLocation();

	// Line trace against the specified collision channel (walls, obstacles)
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartPos,
		EndPos,
		LineOfSightChannel,
		TraceParams
	);

	return !bHit; // Clear line of sight if no obstacle was hit
}

bool UMIR_AlignWithPlayer::CalculateAlignmentTarget(APawn* ControlledPawn, AActor* PlayerActor, FVector& OutTarget, float& OutOffAxisDistance, float& OutCardinalYaw) const
{
	if (!ControlledPawn || !PlayerActor)
	{
		return false;
	}

	FVector EnemyLocation = ControlledPawn->GetActorLocation();
	FVector PlayerLocation = PlayerActor->GetActorLocation();
	UWorld* World = ControlledPawn->GetWorld();

	float DeltaX = FMath::Abs(EnemyLocation.X - PlayerLocation.X);
	float DeltaY = FMath::Abs(EnemyLocation.Y - PlayerLocation.Y);

	// Candidate A: Firing lane along X-axis (slide along Y to match Player.Y)
	FVector TargetX = FVector(EnemyLocation.X, PlayerLocation.Y, EnemyLocation.Z);
	float DistanceX = DeltaY;
	float YawX = (PlayerLocation.X >= EnemyLocation.X) ? 0.0f : 180.0f;

	// Candidate B: Firing lane along Y-axis (slide along X to match Player.X)
	FVector TargetY = FVector(PlayerLocation.X, EnemyLocation.Y, EnemyLocation.Z);
	float DistanceY = DeltaX;
	float YawY = (PlayerLocation.Y >= EnemyLocation.Y) ? 90.0f : -90.0f;

	struct FAlignmentCandidate
	{
		FVector Target;
		float OffAxisDist;
		float Yaw;
	};

	FAlignmentCandidate Primary;
	FAlignmentCandidate Secondary;

	// Primary axis is the more distant axis per GDD
	if (DeltaX >= DeltaY)
	{
		Primary = { TargetX, DistanceX, YawX };
		Secondary = { TargetY, DistanceY, YawY };
	}
	else
	{
		Primary = { TargetY, DistanceY, YawY };
		Secondary = { TargetX, DistanceX, YawX };
	}

	if (!bRequireLineOfSight)
	{
		OutTarget = Primary.Target;
		OutOffAxisDistance = Primary.OffAxisDist;
		OutCardinalYaw = Primary.Yaw;
		return true;
	}

	// 1. Check if Primary candidate provides line of sight to player
	if (HasLineOfSight(World, Primary.Target, PlayerActor, ControlledPawn))
	{
		OutTarget = Primary.Target;
		OutOffAxisDistance = Primary.OffAxisDist;
		OutCardinalYaw = Primary.Yaw;
		return true;
	}

	// 2. Fall back to Secondary candidate if it has line of sight
	if (HasLineOfSight(World, Secondary.Target, PlayerActor, ControlledPawn))
	{
		OutTarget = Secondary.Target;
		OutOffAxisDistance = Secondary.OffAxisDist;
		OutCardinalYaw = Secondary.Yaw;
		return true;
	}

	// Neither candidate alignment position has line of sight to the player
	return false;
}

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

	FVector AlignmentTarget;
	float OffAxisDistance = 0.0f;
	float CardinalYaw = 0.0f;

	// Only consider alignment targets that offer a clear line of sight
	if (!CalculateAlignmentTarget(ControlledPawn, PlayerActor, AlignmentTarget, OffAxisDistance, CardinalYaw))
	{
		return EBTNodeResult::Failed;
	}

	// 3. Check if already precisely aligned within tolerance
	if (OffAxisDistance <= AlignmentTolerance)
	{
		// Verify line of sight from actual current location before succeeding
		if (bRequireLineOfSight && !HasLineOfSight(ControlledPawn->GetWorld(), ControlledPawn->GetActorLocation(), PlayerActor, ControlledPawn))
		{
			return EBTNodeResult::Failed;
		}

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

	FVector AlignmentTarget;
	float OffAxisDistance = 0.0f;
	float CardinalYaw = 0.0f;

	// Dynamically re-evaluate valid line-of-sight alignment targets
	if (!CalculateAlignmentTarget(ControlledPawn, PlayerActor, AlignmentTarget, OffAxisDistance, CardinalYaw))
	{
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 2. Check if we have achieved precise alignment
	if (OffAxisDistance <= AlignmentTolerance)
	{
		AIController->StopMovement();

		// Verify line of sight from actual current location before succeeding
		if (bRequireLineOfSight && !HasLineOfSight(ControlledPawn->GetWorld(), ControlledPawn->GetActorLocation(), PlayerActor, ControlledPawn))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

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
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
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
