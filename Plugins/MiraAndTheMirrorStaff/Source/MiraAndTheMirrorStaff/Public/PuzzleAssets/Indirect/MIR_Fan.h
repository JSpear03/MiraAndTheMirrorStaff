// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleAssets/Indirect/MIR_PuzzleAssetsIndirectBASE.h"
#include "Components/SkeletalMeshComponent.h"
#include "MIR_Fan.generated.h"

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_Fan : public AMIR_PuzzleAssetsIndirectBASE
{
	GENERATED_BODY()


public:
	// Needed a skeleton mesh to replace the static mesh, so it can play fan animation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		USkeletalMeshComponent* SkeletonMesh;

	// The length of the fan's windbox.
	// Also dictates the length of its dedicated Niagara System.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fan")
		float FanLength = 980;

	AMIR_Fan();
};
