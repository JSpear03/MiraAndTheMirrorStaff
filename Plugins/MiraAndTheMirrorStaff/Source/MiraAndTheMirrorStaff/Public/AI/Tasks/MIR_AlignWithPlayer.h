// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MIR_AlignWithPlayer.generated.h"

/**
 * Behavior Tree task for the Mini-Dragon's unique alignment behavior.
 *
 * Per the GDD, the Mini-Dragon "tries to align itself with the player on either
 * the x- or y-axis (whichever is more distant from the player)." Once aligned
 * on that axis, the task succeeds so that the next BT node can fire the ranged
 * projectile attack.
 *
 * The task uses AIController::MoveToLocation to navigate to the alignment point.
 * If the enemy is already within AlignmentTolerance of the target axis, the task
 * succeeds immediately.
 */
struct FBTAlignWithPlayerMemory
{
	float ElapsedTime = 0.0f;
};

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API UMIR_AlignWithPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	// Initiates movement toward the calculated alignment point
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Polls each frame to check whether the MoveToLocation request has completed
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Called when the task is externally aborted (e.g., a higher-priority branch fires)
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTAlignWithPlayerMemory); }

public:
	// Default Constructor
	UMIR_AlignWithPlayer();

	/**
	 * How close (in Unreal units) the enemy's position must be to the target
	 * axis before it is considered "aligned." A small tolerance prevents the
	 * AI from micro-adjusting forever.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alignment",
			  meta = (ClampMin = "10.0", UIMin = "10.0"))
	float AlignmentTolerance = 50.0f;

	/**
	 * How close (in Unreal units) the enemy must get to its calculated
	 * alignment destination before MoveToLocation is considered complete.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alignment",
			  meta = (ClampMin = "5.0", UIMin = "5.0"))
	float AcceptableRadius = 30.0f;

	/**
	 * Maximum time (in seconds) the enemy will spend trying to align before
	 * the task gives up and succeeds anyway (prevents indefinite stalling).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alignment",
			  meta = (ClampMin = "0.5", UIMin = "0.5"))
	float AlignmentTimeout = 4.0f;

	/** Whether alignment requires a clear line of sight to the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alignment")
	bool bRequireLineOfSight = true;

	/** Collision channel used to test line of sight against walls and obstacles */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alignment")
	TEnumAsByte<ECollisionChannel> LineOfSightChannel = ECC_Visibility;

	// Overrides the text drawn on the node inside the Behavior Tree graph
	virtual FString GetStaticDescription() const override;

private:
	/** Tests if a clear line of sight exists between a candidate position and the target */
	bool HasLineOfSight(UWorld* World, const FVector& FromLocation, const AActor* TargetActor, const AActor* IgnoredPawn) const;

	/** Calculates the best alignment destination that has a clear line of sight to the player */
	bool CalculateAlignmentTarget(APawn* ControlledPawn, AActor* PlayerActor, FVector& OutTarget, float& OutOffAxisDistance, float& OutCardinalYaw) const;
};
