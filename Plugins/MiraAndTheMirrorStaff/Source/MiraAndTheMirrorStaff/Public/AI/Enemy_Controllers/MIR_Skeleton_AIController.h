// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MIR_Skeleton_AIController.generated.h"

class UBehaviorTree;

/**
 * AI Controller for Skeleton enemy types.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_Skeleton_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AMIR_Skeleton_AIController();

	/** Blackboard key name for the target actor (defaults to "TargetActor"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	FName TargetActorKeyName;

	/** Finds the player character and sets it to the Blackboard's TargetActor key. */
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	bool AssignPlayerToTargetActor();

	virtual bool RunBehaviorTree(UBehaviorTree* BTAsset) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
