// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"

// Sets default values
AMIR_PuzzleAssetsIndirectBASE::AMIR_PuzzleAssetsIndirectBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetsIndirectBASE::BeginPlay()
{
	Super::BeginPlay();

	//OnIndirectLinked.AddDynamic(this, &AMIR_PuzzleAssetsIndirectBASE::ActivateIndirect);
	//
	//switch (PuzzleState)
	//{
	//case EPuzzleState::Activate:
	//	ActivateIndirect();
	//	break;
	//case EPuzzleState::Deactivate:
	//	DeactivateIndirect();
	//	break;
	//default:
	//	break;
	//}
}

// Called every frame
void AMIR_PuzzleAssetsIndirectBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AMIR_PuzzleAssetsIndirectBASE::CallActivateDelegate()
//{
//	OnIndirectLinked.Broadcast();
//}

