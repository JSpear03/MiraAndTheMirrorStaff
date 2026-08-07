#include "Tasks/MIR_CheckMeleeRange.h"

EBTNodeResult::Type UMIR_CheckMeleeRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Grab all necessary references, return failed if not found
	
	//Get a generic pawn actor
	APawn* GenericPawn = OwnerComp.GetAIOwner()->GetPawn();

	//Convert to actual enemy class
	AMIR_EnemyBASE* ControlledPawn = Cast<AMIR_EnemyBASE>(GenericPawn);

	if (!ControlledPawn) {
		return EBTNodeResult::Failed;
	}

	//Grab the player

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) {
		return EBTNodeResult::Failed;
	}

	//Store the Player Character Reference
	AMIR_PlayerChar* CharacterActor = Cast<AMIR_PlayerChar>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));

	if (!CharacterActor) {
		return EBTNodeResult::Failed;
	}

	//Grab both vectors
	FVector SkeletonLocation = ControlledPawn->GetActorLocation();
	FVector PlayerLocation = CharacterActor->GetActorLocation();

	//Calculate Distance between objects. Using Squared values to be more accurate
	float DistanceSq = FVector::DistSquared(SkeletonLocation, PlayerLocation);

	//Square the attack range for performance improvements
	float AttackRangeSq = FMath::Square(AttackRange);

	if (DistanceSq <= AttackRangeSq) {
		//Inside the range, success!
		return EBTNodeResult::Succeeded;
	}

	//Outside the range, fail!
	return EBTNodeResult::Failed;
}

UMIR_CheckMeleeRange::UMIR_CheckMeleeRange(){
	//Set the name of the node so it shows up as other than "Blackboard Base"
	NodeName = "Check Melee Range";
}
