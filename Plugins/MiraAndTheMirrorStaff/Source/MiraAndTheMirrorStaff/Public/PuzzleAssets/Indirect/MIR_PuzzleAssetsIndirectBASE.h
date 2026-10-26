// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleAssets/MIR_PuzzleAssetBASE.h"
#include "MIR_PuzzleAssetsIndirectBASE.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIndirectLinked);

/**
* An Indirect Puzzle Asset (IPA) is a puzzle asset that is activated or deactivated by
* a Puzzle-Asset Manager (PAM) based on whether its linked Direct Puzzle Assets (DPAs)
* have been deactivated or activated.
* 
* Thus, IPAs represent the "effect" in the cause–effect relationship between DPAs and IPAs.
*/
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PuzzleAssetsIndirectBASE : public AMIR_PuzzleAssetBASE
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AMIR_PuzzleAssetsIndirectBASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
