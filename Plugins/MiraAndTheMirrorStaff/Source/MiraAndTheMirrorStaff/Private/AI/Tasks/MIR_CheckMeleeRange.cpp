#include "AI/Tasks/MIR_CheckMeleeRange.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"


UMIR_CheckMeleeRange::UMIR_CheckMeleeRange()
{
	NodeName = "Check Melee Range";
	AttackRange = 200.0f;

	// Filter target key selector to only accept Actors/Objects (e.g. Player character)
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMIR_CheckMeleeRange, BlackboardKey), AActor::StaticClass());

	// Filter bool key selector to only accept Booleans (e.g. WithinMeleeRange)
	WithinMeleeRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UMIR_CheckMeleeRange, WithinMeleeRangeKey));
	WithinMeleeRangeKey.SelectedKeyName = FName("WithinMeleeRange");
}

FString UMIR_CheckMeleeRange::GetStaticDescription() const
{
	FName BoolKey = WithinMeleeRangeKey.SelectedKeyName.IsNone() ? FName("WithinMeleeRange") : WithinMeleeRangeKey.SelectedKeyName;
	return FString::Printf(TEXT("%s\nRange: %.1f\nTarget: %s\nSets Bool: %s"),
		*Super::GetStaticDescription(),
		AttackRange,
		*BlackboardKey.SelectedKeyName.ToString(),
		*BoolKey.ToString());
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

	FName BoolKeyName = WithinMeleeRangeKey.SelectedKeyName.IsNone() ? FName("WithinMeleeRange") : WithinMeleeRangeKey.SelectedKeyName;

	// 1. Try to get target player from Blackboard key (if selected)
	AMIR_PlayerChar* CharacterActor = Cast<AMIR_PlayerChar>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));

	// 2. Fallback to Player 0 if Blackboard key is not set or not pointing to player
	if (!CharacterActor)
	{
		CharacterActor = Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(ControlledPawn->GetWorld(), 0));
	}

	if (!CharacterActor)
	{
		// Player not found -> explicitly set bool to false
		BlackboardComp->SetValueAsBool(BoolKeyName, false);
		return EBTNodeResult::Failed;
	}


	// Calculate distance between enemy and player
	float DistanceSq = FVector::DistSquared(ControlledPawn->GetActorLocation(), CharacterActor->GetActorLocation());
	float AttackRangeSq = FMath::Square(AttackRange);
	bool bIsInRange = (DistanceSq <= AttackRangeSq);

	// Actively write the in-range result to the Blackboard bool key
	BlackboardComp->SetValueAsBool(BoolKeyName, bIsInRange);

	if (bIsInRange)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


