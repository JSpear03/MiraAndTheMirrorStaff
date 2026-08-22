#include "AI/Tasks/MIR_CheckMeleeRange.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UMIR_CheckMeleeRange::UMIR_CheckMeleeRange()
{
	NodeName = "Check Melee Range";
	AttackRange = 200.0f;

	// Filter blackboard selector to only accept Actors/Objects (e.g. Player character)
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMIR_CheckMeleeRange, BlackboardKey), AActor::StaticClass());
}

FString UMIR_CheckMeleeRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s\nRange: %.1f\nTarget: %s"),
		*Super::GetStaticDescription(),
		AttackRange,
		*BlackboardKey.SelectedKeyName.ToString());
}

EBTNodeResult::Type UMIR_CheckMeleeRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* GenericPawn = AIController->GetPawn();
	AMIR_EnemyBASE* ControlledPawn = Cast<AMIR_EnemyBASE>(GenericPawn);
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AMIR_PlayerChar* CharacterActor = Cast<AMIR_PlayerChar>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!CharacterActor)
	{
		return EBTNodeResult::Failed;
	}

	// Calculate distance between enemy and player
	float DistanceSq = FVector::DistSquared(ControlledPawn->GetActorLocation(), CharacterActor->GetActorLocation());
	float AttackRangeSq = FMath::Square(AttackRange);

	if (DistanceSq <= AttackRangeSq)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

