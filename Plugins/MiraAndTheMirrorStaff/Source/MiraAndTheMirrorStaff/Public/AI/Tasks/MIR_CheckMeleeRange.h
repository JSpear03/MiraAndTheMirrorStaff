// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Actor.h"
#include "AI/Enemy_Controllers/MIR_Skeleton_AIController.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MIR_CheckMeleeRange.generated.h" 


/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class MIRAANDTHEMIRRORSTAFF_API UMIR_CheckMeleeRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	//Execute Task
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	//Exposes the configurable distance between the skeleton and the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float AttackRange;

	//Default Constructor
	UMIR_CheckMeleeRange();
	
};
