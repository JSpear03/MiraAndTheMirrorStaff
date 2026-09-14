#include "AI/Enemy_Controllers/MIR_Skeleton_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AMIR_Skeleton_AIController::AMIR_Skeleton_AIController()
{
	TargetActorKeyName = FName("TargetActor");
}

void AMIR_Skeleton_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AssignPlayerToTargetActor();
}

bool AMIR_Skeleton_AIController::RunBehaviorTree(UBehaviorTree* BTAsset)
{
	bool bSuccess = Super::RunBehaviorTree(BTAsset);
	if (bSuccess)
	{
		AssignPlayerToTargetActor();
	}
	return bSuccess;
}

bool AMIR_Skeleton_AIController::AssignPlayerToTargetActor()
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerChar)
	{
		return false;
	}

	BlackboardComp->SetValueAsObject(TargetActorKeyName, PlayerChar);
	return true;
}

