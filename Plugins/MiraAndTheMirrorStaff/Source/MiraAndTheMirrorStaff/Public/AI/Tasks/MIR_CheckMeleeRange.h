// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MIR_CheckMeleeRange.generated.h"

/**
 * Behavior Tree task that checks whether the target player is within melee attack range.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API UMIR_CheckMeleeRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	// Execute Task
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// Default Constructor
	UMIR_CheckMeleeRange();

	// Exposes the configurable distance between the enemy and the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float AttackRange = 200.0f;

	// Blackboard bool key to update with in-range status (defaults to WithinMeleeRange)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector WithinMeleeRangeKey;

	// Overrides the text drawn on the node inside the Behavior Tree graph
	virtual FString GetStaticDescription() const override;
};


