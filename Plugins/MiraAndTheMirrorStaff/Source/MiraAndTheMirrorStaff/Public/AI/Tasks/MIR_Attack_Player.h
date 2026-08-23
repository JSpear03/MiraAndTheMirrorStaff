// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "AI/Enemy_Controllers/MIR_Skeleton_AIController.h"
#include "Components/MIR_Combat_Component.h"
#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MIR_Attack_Player.generated.h"

/**
 * 
 */

//Needed strictly for the Behavior Tree, not for any magic or specific combat.
UENUM(BlueprintType)
enum class ECombatAttackMode : uint8
{
	Melee    UMETA(DisplayName = "Melee Mode"),
	Magic   UMETA(DisplayName = "Magic Mode")
};

struct FBTAttackPlayerMemory
{
	float ElapsedTime = 0.0f;
};

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API UMIR_Attack_Player : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	// Starts the attack sequence
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Polls every frame to check if the attack/deactivation sequence is complete - Needed to check if death overwrites action
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTAttackPlayerMemory); }

public:

	// Default Constructor
	UMIR_Attack_Player();

	// Exposes a clean configuration dropdown in the Behavior Tree Details Panel
	UPROPERTY(EditAnywhere, Category = "AI|Combat")
	ECombatAttackMode SelectedAttackMode;

	// Maximum time (in seconds) to wait for FinishCurrentAttack before timing out and succeeding (prevents AI lockup)
	UPROPERTY(EditAnywhere, Category = "AI|Combat", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float AttackTimeout = 2.0f;

	// Overrides the text drawn on the node inside the Behavior Tree graph
	virtual FString GetStaticDescription() const override;
};

